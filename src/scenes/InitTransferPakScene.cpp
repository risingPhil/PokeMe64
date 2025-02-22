#include "scenes/InitTransferPakScene.h"
#include "scenes/MenuScene.h"
#include "scenes/SceneManager.h"
#include "transferpak/TransferPakManager.h"
#include "transferpak/TransferPakRomReader.h"
#include "transferpak/TransferPakSaveManager.h"
#include "gen1/Gen1GameReader.h"
#include "gen2/Gen2GameReader.h"
#include "menu/MenuEntries.h"

#include <unistd.h>

static const Rectangle tpakDetectWidgetBounds = {60, 44, 200, 116};


static void dialogFinishedCallback(void* context)
{
    InitTransferPakScene* scene = (InitTransferPakScene*)context;
    scene->onDialogDone();
}

static void tpakWidgetStateChangedCallback(void* context, TransferPakWidgetState newState)
{
    InitTransferPakScene* scene = (InitTransferPakScene*)context;
    scene->onTransferPakWidgetStateChanged(newState);
}

InitTransferPakScene::InitTransferPakScene(SceneDependencies& deps, void*)
    : SceneWithDialogWidget(deps)
    , menu9SliceSprite_(nullptr)
    , tpakDetectWidget_(deps.animationManager, deps.tpakManager)
    , tpakDetectWidgetSegment_(WidgetFocusChainSegment{
        .current = &tpakDetectWidget_
    })
    , diagData_({0})
    , pokeMe64TextSettings_()
    , gameTypeString_(nullptr)
{
}

InitTransferPakScene::~InitTransferPakScene()
{
}

void InitTransferPakScene::init()
{
    uint8_t systemEntropy[4];
    menu9SliceSprite_ = sprite_load("rom://menu-bg-9slice.sprite");

    SceneWithDialogWidget::init();

    setupTPakDetectWidget();
    setFocusChain(&tpakDetectWidgetSegment_);

    pokeMe64TextSettings_ = TextRenderSettings{
        .fontId = mainFontId_,
        .fontStyleId = fontStyleWhiteId_,
        .halign = ALIGN_CENTER
    };

    // add some entropy for our rand() function.
    // we'll apply this later. Right before we go to the main menu.
    getentropy(systemEntropy, sizeof(systemEntropy));
    for(uint8_t i=0; i < sizeof(systemEntropy); ++i)
    {
        randomSeed_ += static_cast<unsigned int>(systemEntropy[i]) << (i * 8);
    }
}

void InitTransferPakScene::destroy()
{
    SceneWithDialogWidget::destroy();
    
    sprite_free(menu9SliceSprite_);
    menu9SliceSprite_ = nullptr;
}

void InitTransferPakScene::render(RDPQGraphics& gfx, const Rectangle& sceneBounds)
{
    gfx.drawText(Rectangle{0, 10, 320, 16}, "PokeMe64 by risingPhil. Version 0.3", pokeMe64TextSettings_);
    tpakDetectWidget_.render(gfx, sceneBounds);

    SceneWithDialogWidget::render(gfx, sceneBounds);
}

void InitTransferPakScene::onDialogDone()
{
    MenuSceneContext* menuContext = nullptr;

    if(tpakDetectWidget_.getState() == TransferPakWidgetState::NO_SAVE_FOUND)
    {
        menuContext = new MenuSceneContext{
            .menuEntries = backupRestoreMenuEntries,
            .numMenuEntries = static_cast<uint32_t>(backupRestoreMenuEntriesSize / sizeof(backupRestoreMenuEntries[0])),
            .bButtonMeansUserWantsToSwitchCartridge = true
        };
    }
    else if(tpakDetectWidget_.getState() == TransferPakWidgetState::VALID_SAVE_FOUND)
    {
        Gen1GameType gen1Type;
        Gen2GameType gen2Type;

        tpakDetectWidget_.retrieveGameType(gen1Type, gen2Type);

        if(gen1Type != Gen1GameType::INVALID)
        {
            menuContext = new MenuSceneContext({
                .menuEntries = gen1MenuEntries,
                .numMenuEntries = static_cast<uint32_t>(gen1MenuEntriesSize / sizeof(gen1MenuEntries[0])),
                .bButtonMeansUserWantsToSwitchCartridge = true
            });
        }
        else if(gen2Type != Gen2GameType::INVALID)
        {
            if(gen2Type == Gen2GameType::CRYSTAL)
            {
                menuContext = new MenuSceneContext({
                    .menuEntries = gen2CrystalMenuEntries,
                    .numMenuEntries = static_cast<uint32_t>(gen2CrystalMenuEntriesSize / sizeof(gen2CrystalMenuEntries[0])),
                    .bButtonMeansUserWantsToSwitchCartridge = true
                });
            }
            else
            {
                menuContext = new MenuSceneContext({
                    .menuEntries = gen2MenuEntries,
                    .numMenuEntries = static_cast<uint32_t>(gen2MenuEntriesSize / sizeof(gen2MenuEntries[0])),
                    .bButtonMeansUserWantsToSwitchCartridge = true
                });
            }
        }
    }
    else
    {
        debugf("ERROR: Not gen 1 nor gen 2. This shouldn't be happening!\r\n");
        return;
    }

    // add the number of ticks it took before we got to this point to our random seed
    randomSeed_ += static_cast<uint32_t>(get_ticks());
    // Now apply random seed to srand() before we get to the main menu.
    srand(randomSeed_);

    deps_.sceneManager.switchScene(SceneType::MENU, deleteMenuSceneContext, menuContext);
}

void InitTransferPakScene::onTransferPakWidgetStateChanged(TransferPakWidgetState newState)
{
    debugf("onTransferPakWidgetStateChanged(%d)\r\n", static_cast<int>(newState));
    if(newState == TransferPakWidgetState::VALID_SAVE_FOUND)
    {
        debugf("[InitTransferPakScene]: Game found!\r\n");
        loadGameType();
        deps_.tpakManager.setRAMEnabled(true);
        loadSaveMetadata();
        /* Quote:
         * "It is recommended to disable external RAM after accessing it, in order to protect its contents from corruption
         *  during power down of the Game Boy or removal of the cartridge. Once the cartridge has completely lost power from
         *  the Game Boy, the RAM is automatically disabled to protect it."
         * 
         * source: https://gbdev.io/pandocs/MBC1.html 
         * 
         * Yes, I'm aware we're dealing with MBC3 here, but there's some overlap and what applies to MBC1 here likely also applies
         * to MBC3
         */
        deps_.tpakManager.setRAMEnabled(false); 

        setDialogDataText(diagData_, "Hi %s! We've detected Pokémon %s in the N64 Transfer Pak. Let's go!", deps_.playerName, gameTypeString_);
        dialogWidget_.appendDialogData(&diagData_);
        dialogWidget_.setVisible(true);
        setFocusChain(&dialogFocusChainSegment_);
    }
    else if(newState == TransferPakWidgetState::NO_SAVE_FOUND)
    {
        debugf("[InitTransferPakScene]: Game found, but no save found!\r\n");
        loadGameType();

        setDialogDataText(diagData_, "We can't find a save in your Pokemon %s cartridge. You'll only be able to backup/restore!", gameTypeString_);
        dialogWidget_.appendDialogData(&diagData_);
        dialogWidget_.setVisible(true);
        setFocusChain(&dialogFocusChainSegment_);
    }
    else
    {
        switch(newState)
        {
        case TransferPakWidgetState::GB_HEADER_VALIDATION_FAILED:
        case TransferPakWidgetState::NO_GAME_FOUND:
        case TransferPakWidgetState::NO_TRANSFER_PAK_FOUND:
            setDialogDataText(diagData_, "We could not find a suitable game cartridge! Please turn the console off and try again!");
            diagData_.userAdvanceBlocked = true;
            dialogWidget_.appendDialogData(&diagData_);
            dialogWidget_.setVisible(true);
            break;
        default:
            break;
        }
    }
}

void InitTransferPakScene::setupTPakDetectWidget()
{
    const TransferPakDetectionWidgetStyle style = {
        .textSettings = {
            .fontId = mainFontId_,
            .fontStyleId = fontStyleWhiteId_,
            .halign = ALIGN_CENTER
        }
    };

    tpakDetectWidget_.setStyle(style);
    tpakDetectWidget_.setStateChangedCallback(tpakWidgetStateChangedCallback, this);
    tpakDetectWidget_.setBounds(tpakDetectWidgetBounds);
}

void InitTransferPakScene::setupDialog(DialogWidgetStyle& style)
{
    style.background.sprite = menu9SliceSprite_;
    style.background.spriteSettings = {
        .renderMode = SpriteRenderMode::NINESLICE,
        .srcRect = { 6, 6, 6, 6 }
    };

    SceneWithDialogWidget::setupDialog(style);

    dialogWidget_.setOnDialogFinishedCallback(dialogFinishedCallback, this);
    dialogWidget_.setVisible(false);
}

void InitTransferPakScene::loadGameType()
{
    Gen1GameType gen1Type;
    Gen2GameType gen2Type;
    tpakDetectWidget_.retrieveGameType(gen1Type, gen2Type);

    if(gen1Type != Gen1GameType::INVALID)
    {
        deps_.generation = 1;
        deps_.specificGenVersion = static_cast<uint8_t>(gen1Type);

        switch(gen1Type)
        {
            case Gen1GameType::BLUE:
                gameTypeString_ = "Blue";
                break;
            case Gen1GameType::RED:
                gameTypeString_ = "Red";
                break;
            case Gen1GameType::GREEN:
                gameTypeString_ = "Green";
                break;
            case Gen1GameType::YELLOW:
                gameTypeString_ = "Yellow";
                break;
            default:
                gameTypeString_ = "";
                break;
        }
    }
    else if(gen2Type != Gen2GameType::INVALID)
    {
        deps_.generation = 2;
        deps_.specificGenVersion = static_cast<uint8_t>(gen2Type);

        switch(gen2Type)
        {
            case Gen2GameType::GOLD:
                gameTypeString_ = "Gold";
                break;
            case Gen2GameType::SILVER:
                gameTypeString_ = "Silver";
                break;
            case Gen2GameType::CRYSTAL:
                gameTypeString_ = "Crystal";
                break;
            default:
                gameTypeString_ = "";
                break;
        }
    }
}

void InitTransferPakScene::loadSaveMetadata()
{
    TransferPakRomReader romReader(deps_.tpakManager);
    TransferPakSaveManager saveManager(deps_.tpakManager);
    Gen1GameType gen1Type;
    Gen2GameType gen2Type;
    uint16_t trainerID = 0;

    tpakDetectWidget_.retrieveGameType(gen1Type, gen2Type);

    if(gen1Type != Gen1GameType::INVALID)
    {
        const Gen1LocalizationLanguage language = gen1_determineGameLanguage(romReader, gen1Type);

        Gen1GameReader gameReader(romReader, saveManager, gen1Type, language);
        const char* trainerName = gameReader.getTrainerName();
        trainerID = gameReader.getTrainerID();
        deps_.localization = static_cast<uint8_t>(language);
        strncpy(deps_.playerName, trainerName, sizeof(deps_.playerName) - 1);
    }
    else if(gen2Type != Gen2GameType::INVALID)
    {
        const Gen2LocalizationLanguage language = gen2_determineGameLanguage(romReader, gen2Type);

        Gen2GameReader gameReader(romReader, saveManager, gen2Type, language);
        const char* trainerName = gameReader.getTrainerName();
        trainerID = gameReader.getTrainerID();
        deps_.localization = static_cast<uint8_t>(language);
        if(language != Gen2LocalizationLanguage::KOREAN)
        {
            strncpy(deps_.playerName, trainerName, sizeof(deps_.playerName) - 1);
        }
        else
        {
            const char fakePlayerName[] = "Trainer";
            memcpy(deps_.playerName, fakePlayerName, sizeof(fakePlayerName));
        }
    }

    // add to our random seed to make sure our random numbers will be randomized.
    randomSeed_ += trainerID;
    for(uint8_t i=0; i < strlen(deps_.playerName); ++i)
    {
        randomSeed_ += deps_.playerName[i];
    }
}
