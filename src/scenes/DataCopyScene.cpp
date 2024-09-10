#include "scenes/DataCopyScene.h"
#include "core/DragonUtils.h"
#include "scenes/SceneManager.h"
#include "menu/MenuFunctions.h"
#include "gen1/Gen1Common.h"
#include "gen2/Gen2Common.h"

#include <system.h>

//missing function declaration in libdragons' system.h, but the definition exists in system.c
int mkdir( const char * path, mode_t mode );

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

/**
 * @brief The reason this function exists is because I first tried to use the cartridge header title + trainerName (max 11 chars) + unique number as the game save filename
 * but it turned out being a bit too close to fill the entire second line of the DialogWidget because the path became too long.
 * The solution is to just create a shorter game title (just "Blue" or "Red" or "Crystal"). That frees up some room in the DialogWidget
 * for the trainername and save number
 */
static void generateRomTitle(char* outputPath, const gameboy_cartridge_header& gbHeader, uint8_t generation, uint8_t specificGenVersion)
{
    if(generation == 1)
    {
        switch(static_cast<Gen1GameType>(specificGenVersion))
        {
            case Gen1GameType::BLUE:
                strcpy(outputPath, "Blue");
                break;
            case Gen1GameType::RED:
                strcpy(outputPath, "Red");
                break;
            case Gen1GameType::YELLOW:
                strcpy(outputPath, "Yellow");
                break;
            default:
                strcpy(outputPath, "Unknown");
                break;
        }
    }
    else if(generation == 2)
    {
        switch(static_cast<Gen2GameType>(specificGenVersion))
        {
            case Gen2GameType::GOLD:
                strcpy(outputPath, "Gold");
                break;
            case Gen2GameType::SILVER:
                strcpy(outputPath, "Silver");
                break;
            case Gen2GameType::CRYSTAL:
                strcpy(outputPath, "Crystal");
                break;
            default:
                strcpy(outputPath, "Unknown");
                break;
        }   
    }
    else
    {
        // the title field of the gameboy header is likely truncated.
        // create a copy and make sure to append a null character so we won't crash when trying to use it as a string
        memcpy(outputPath, gbHeader.new_title.title, 11);
        outputPath[11] = '\0';
    }
}

static void generateSaveFileName(char* savOutputPath, size_t bufferSize, const char* gameTitle, const char* playerName)
{
    struct stat statStruct;
    unsigned uniqueNumber = 0;
    const size_t playerNameSize = strlen(playerName);

    if(playerNameSize)
    {
        snprintf(savOutputPath, bufferSize - 1, "sd:/PokeMe64/%s_%s.sav", gameTitle, playerName);
    }
    else
    {
        snprintf(savOutputPath, bufferSize - 1, "sd:/PokeMe64/%s.sav", gameTitle);
    }

    while(stat(savOutputPath, &statStruct) == 0)
    {
        if(playerNameSize)
        {
            snprintf(savOutputPath, bufferSize - 1, "sd:/PokeMe64/%s_%s_%u.sav", gameTitle, playerName, uniqueNumber);
        }
        else
        {
            snprintf(savOutputPath, bufferSize - 1, "sd:/PokeMe64/%s_%u.sav", gameTitle, uniqueNumber);
        }
        ++uniqueNumber;
    }
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
    char savOutputPath[4096];
    char romOutputPath[4096];
    char gameTitle[12];
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

    mkdir("sd:/PokeMe64", 0777);

    gameboy_cartridge_header gbHeader;
    deps_.tpakManager.readCartridgeHeader(gbHeader);

    generateRomTitle(gameTitle, gbHeader, deps_.generation, deps_.specificGenVersion);

    auto msg2 = new DialogData{
        .shouldDeleteWhenDone = true
    };

    switch(sceneContext_->operation)
    {
        case DataCopyOperation::BACKUP_SAVE:
            generateSaveFileName(savOutputPath, sizeof(savOutputPath), gameTitle, deps_.playerName);
            copySource_ = new TransferPakSaveManagerCopySource(saveManager_);
            copyDestination_ = new TransferPakFileCopyDestination(savOutputPath);
            totalBytesToCopy_ = convertSRAMSizeIntoNumBytes(gbHeader.ram_size_code);
            setDialogDataText(*msg2, "The save was backed up to %s!", savOutputPath);
            break;
        case DataCopyOperation::BACKUP_ROM:
            snprintf(romOutputPath, sizeof(savOutputPath) - 1, "sd:/PokeMe64/%s.gbc", gameTitle);
            copySource_ = new TransferPakRomReaderCopySource(romReader_);
            copyDestination_ = new TransferPakFileCopyDestination(romOutputPath);
            totalBytesToCopy_ = convertROMSizeIntoNumBytes(gbHeader.rom_size_code);
            setDialogDataText(*msg2, "The cartridge rom was backed up to %s!", romOutputPath);
            break;
        case DataCopyOperation::RESTORE_SAVE:
            copySource_ = new TransferPakFileCopySource(sceneContext_->saveToRestorePath.get());
            copyDestination_ = new TransferPakSaveManagerDestination(saveManager_);
            totalBytesToCopy_ = convertSRAMSizeIntoNumBytes(gbHeader.ram_size_code);
            setDialogDataText(*msg2, "The save was restored to the cartridge!", romOutputPath);
            break;
    }

    if(!copySource_->readyForTransfer())
    {
        if(sceneContext_->operation == DataCopyOperation::RESTORE_SAVE)
        {
            setDialogDataText(diag_, "ERROR: Could not read from file %s!", sceneContext_->saveToRestorePath.get());
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