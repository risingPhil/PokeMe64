#include "scenes/DataCopyScene.h"
#include "core/DragonUtils.h"
#include "scenes/SceneManager.h"
#include "menu/MenuFunctions.h"

/**
 * Copying from or to the transfer pak is a blocking operation.
 * So while we're doing that, we can't render anything.
 * 
 * So, in order to not just entirely freeze until the copy operation is done, we copy
 * in chunks. 
 * 
 * We know from http://n64devkit.square7.ch/pro-man/pro26/26-07.htm
 * that the transfer pak is able to read 32 bytes every 1,5 - 2 milliseconds.
 * 
 * Our copy operation at the moment isn't entirely efficient, so it likely is slower.
 * But just basing off that number, 4096 bytes should be transferred within +- 256 ms
 * 
 * That means we would theoretically render at 4fps during the copy with this chunk size.
 * (due to our copy implementation, it's likely less though)
 */
static int COPY_CHUNK_SIZE_IN_BYTES = 4096;

static void dialogFinishedCallback(void* context)
{
    DataCopyScene* scene = (DataCopyScene*)context;
    scene->onDialogDone();
}

DataCopyScene::DataCopyScene(SceneDependencies& deps, void* context)
    : SceneWithProgressBar(deps)
    , romReader_(deps.tpakManager)
    , saveManager_(deps.tpakManager)
    , sceneContext_((DataCopySceneContext*)context)
    , copySource_(nullptr)
    , copyDestination_(nullptr)
    , copier_(nullptr)
    , dialogWidgetSprite_(nullptr)
    , progressBackgroundSprite_(nullptr)
    , diag_({0})
    , totalBytesToCopy_(0)
{
    (void)context;
}

DataCopyScene::~DataCopyScene()
{
}

void DataCopyScene::init()
{
    dialogWidgetSprite_ = sprite_load("rom://menu-bg-9slice.sprite");
    progressBackgroundSprite_ = sprite_load("rom://bg-nineslice-transparant-border.sprite");

    SceneWithProgressBar::init();

    // check if the n64 flashcart is supported
    if(!doesN64FlashCartSupportSDCardAccess())
    {
        setDialogDataText(diag_, "Sorry! This is only supported on 64Drive, Everdrive64, ED64Plus and SummerCart64!");
        showDialog(&diag_);
        return;
    }

    // check if the sd card is mounted
    if(!sdcard_mounted)
    {
        setDialogDataText(diag_, "ERROR: SD card is not mounted!");
        showDialog(&diag_);
        return;
    }

    const char* savOutputPath = "sd:/gb_out.sav";
    const char* romOutputPath = "sd:/rom.gb";

    gameboy_cartridge_header gbHeader;
    deps_.tpakManager.readCartridgeHeader(gbHeader);

    auto msg2 = new DialogData{
        .shouldDeleteWhenDone = true
    };

    switch(sceneContext_->operation)
    {
        case DataCopyOperation::BACKUP_SAVE:
            copySource_ = new TransferPakSaveManagerCopySource(saveManager_);
            copyDestination_ = new TransferPakFileCopyDestination(savOutputPath);
            totalBytesToCopy_ = convertSRAMSizeIntoNumBytes(gbHeader.ram_size_code);
            setDialogDataText(*msg2, "The cartridge save was backed up to %s!", savOutputPath);
            break;
        case DataCopyOperation::BACKUP_ROM:
            copySource_ = new TransferPakRomReaderCopySource(romReader_);
            copyDestination_ = new TransferPakFileCopyDestination(romOutputPath);
            totalBytesToCopy_ = convertROMSizeIntoNumBytes(gbHeader.rom_size_code);
            setDialogDataText(*msg2, "The cartridge rom was backed up to %s!", romOutputPath);
            break;
        case DataCopyOperation::RESTORE_SAVE:
            copySource_ = new TransferPakFileCopySource(savOutputPath);
            copyDestination_ = new TransferPakSaveManagerDestination(saveManager_);
            totalBytesToCopy_ = convertSRAMSizeIntoNumBytes(gbHeader.ram_size_code);
            setDialogDataText(*msg2, "The save file was restored to the cartridge!", romOutputPath);
            break;
    }

    if(!copySource_->readyForTransfer())
    {
        if(sceneContext_->operation == DataCopyOperation::RESTORE_SAVE)
        {
            setDialogDataText(diag_, "ERROR: Could not read from file %s!", savOutputPath);
        }
        else
        {
            setDialogDataText(diag_, "ERROR: Could not read from cartridge!");
        }

        // not needed
        delete msg2;
        msg2 = nullptr;
        
        // now show the error dialog
        showDialog(&diag_);
        return;
    }

    if(!copyDestination_->readyForTransfer())
    {
        if(sceneContext_->operation == DataCopyOperation::RESTORE_SAVE)
        {
            setDialogDataText(diag_, "ERROR: Could not write to cartridge!");
        }
        else
        {
            const char* outputPath = (sceneContext_->operation == DataCopyOperation::BACKUP_SAVE) ? savOutputPath : romOutputPath;
            setDialogDataText(diag_, "ERROR: Could not write to file %s!", outputPath);
        }

        // not needed
        delete msg2;
        msg2 = nullptr;
        // now show the error dialog
        showDialog(&diag_);
        return;
    }

    setDialogDataText(diag_, "Copying. Please Wait...");
    diag_.userAdvanceBlocked = true;
    diag_.next = msg2;
    showDialog(&diag_);

    deps_.tpakManager.setRAMEnabled(true);
    copier_ = new TransferPakDataCopier(*copySource_, *copyDestination_);
}

void DataCopyScene::destroy()
{
    sprite_free(dialogWidgetSprite_);
    dialogWidgetSprite_ = nullptr;
    sprite_free(progressBackgroundSprite_);
    progressBackgroundSprite_ = nullptr;

    if(copier_)
    {
        delete copier_;
        copier_ = nullptr;
    }

    if(copySource_)
    {
        delete copySource_;
        copySource_ = nullptr;
    }

    if(copyDestination_)
    {
        delete copyDestination_;
        copyDestination_ = nullptr;
    }

    deps_.tpakManager.setRAMEnabled(false);
    SceneWithProgressBar::destroy();
}

void DataCopyScene::processUserInput()
{
    if(copier_ && copyDestination_ && copyDestination_->getNumberOfBytesWritten() < totalBytesToCopy_)
    {
        const uint32_t numBytesToCopy = std::min<uint32_t>(COPY_CHUNK_SIZE_IN_BYTES, totalBytesToCopy_ - copyDestination_->getNumberOfBytesWritten());
        copier_->copyChunk(numBytesToCopy);

        setProgress(static_cast<double>(copyDestination_->getNumberOfBytesWritten()) / static_cast<double>(totalBytesToCopy_));
    }

    if(copier_ && copyDestination_ && copyDestination_->getNumberOfBytesWritten() >= totalBytesToCopy_)
    {
        deps_.tpakManager.setRAMEnabled(false);
        copyDestination_->close();
        delete copySource_;
        copySource_ = nullptr;
        delete copyDestination_;
        copyDestination_ = nullptr;
        delete copier_;

        // The copy operation is done, now advance the blocked dialog entry to the final one
        advanceDialog();
    }

    SceneWithProgressBar::processUserInput();
}

void DataCopyScene::render(RDPQGraphics& gfx, const Rectangle& sceneBounds)
{
    SceneWithProgressBar::render(gfx, sceneBounds);
}

void DataCopyScene::onDialogDone()
{
    deps_.sceneManager.goBackToPreviousScene();
}

void DataCopyScene::setupDialog(DialogWidgetStyle& style)
{
    style.background.sprite = dialogWidgetSprite_;
    style.background.spriteSettings = {
        .renderMode = SpriteRenderMode::NINESLICE,
        .srcRect = { 6, 6, 6, 6 }
    };

    SceneWithProgressBar::setupDialog(style);

    dialogWidget_.setOnDialogFinishedCallback(dialogFinishedCallback, this);
    dialogWidget_.setVisible(false);
}

void DataCopyScene::setupProgressBar(ProgressBarWidgetStyle& style)
{
    SceneWithProgressBar::setupProgressBar(style);

    style.background = {
        .sprite = progressBackgroundSprite_,
        .renderSettings = {
            .renderMode = SpriteRenderMode::NINESLICE,
            .srcRect = { 6, 6, 6, 6 }
        }
    };
}

void deleteDataCopySceneContext(void* context)
{
    DataCopySceneContext* sceneContext = (DataCopySceneContext*)context;

    delete sceneContext;
}