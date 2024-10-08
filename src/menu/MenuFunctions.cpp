#include "menu/MenuFunctions.h"
#include "menu/MenuEntries.h"
#include "core/RDPQGraphics.h"
#include "scenes/DistributionPokemonListScene.h"
#include "scenes/StatsScene.h"
#include "scenes/MenuScene.h"
#include "scenes/SelectFileScene.h"
#include "scenes/SceneManager.h"
#include "gen2/Gen2GameReader.h"
#include "transferpak/TransferPakManager.h"
#include "transferpak/TransferPakRomReader.h"
#include "transferpak/TransferPakSaveManager.h"

#define POKEMON_CRYSTAL_ITEM_ID_GS_BALL 0x73

const Move MOVE_SURF = Move::SURF;
const Move MOVE_FLY = Move::FLY;

const DataCopyOperation DATACOPY_BACKUP_SAVE = DataCopyOperation::BACKUP_SAVE;
const DataCopyOperation DATACOPY_BACKUP_ROM = DataCopyOperation::BACKUP_ROM;
const DataCopyOperation DATACOPY_RESTORE_SAVE = DataCopyOperation::RESTORE_SAVE;
const DataCopyOperation DATACOPY_WIPE_SAVE = DataCopyOperation::WIPE_SAVE;

// based on https://github.com/kwsch/PKHeX/blob/master/PKHeX.Core/Resources/text/script/gen2/flags_c_en.txt
const uint16_t GEN2_EVENTFLAG_DECORATION_PIKACHU_BED = 679;
const uint16_t GEN2_EVENTFLAG_DECORATION_UNOWN_DOLL = 712;
const uint16_t GEN2_EVENTFLAG_DECORATION_TENTACOOL_DOLL = 715;

typedef struct Gen1TeachPikachuParams
{
    Move moveType;
    uint8_t partyIndex;
    uint8_t moveIndex;
    Gen1TrainerPokemon poke;
} Gen1TeachPikachuParams;

// here are a few structs I use to pass around parameters to the gen1TeachPikachu() function
// i want to avoid dynamic allocation for this purpose.
// we need 4 of them because we will have 4 menu options, each with a different moveIndex
static Gen1TeachPikachuParams teachParamsMove1 = {
    .moveType = Move::SURF, // dummy value. Will be replaced later
    .moveIndex = 0
};
static Gen1TeachPikachuParams teachParamsMove2 = {
    .moveType = Move::SURF, // dummy value. Will be replaced later
    .moveIndex = 1
};
static Gen1TeachPikachuParams teachParamsMove3 = {
    .moveType = Move::SURF, // dummy value. Will be replaced later
    .moveIndex = 2
};
static Gen1TeachPikachuParams teachParamsMove4 = {
    .moveType = Move::SURF, // dummy value. Will be replaced later
    .moveIndex = 3
};

static void goToDistributionPokemonListMenu(void* context, DistributionPokemonListType type)
{
    auto sceneContext = new DistributionPokemonListSceneContext;
    sceneContext->listType = type;
    sceneContext->bButtonMeansUserWantsToSwitchCartridge = false;
    sceneContext->numMenuEntries = 0;
    sceneContext->menuEntries = nullptr;

    MenuScene* scene = static_cast<MenuScene*>(context);
    SceneManager& sceneManager = scene->getDependencies().sceneManager;

    sceneManager.switchScene(SceneType::DISTRIBUTION_POKEMON_LIST, deleteDistributionPokemonListSceneContext, sceneContext);
}

static uint8_t gen1FindPikachuInParty(Gen1Party& party)
{
    const uint8_t PIKACHU_INDEX_CODE = 0x54;
    uint8_t foundIndex = 0xFF;
    const uint8_t numberOfPokemon = party.getNumberOfPokemon();
    
    for(uint8_t i=0; i < numberOfPokemon; ++i)
    {
        const uint8_t speciesIndex = party.getSpeciesAtIndex(i);
        if(speciesIndex == PIKACHU_INDEX_CODE)
        {
//          debugf("Found Pikachu at index %hu\r\n", i);
            foundIndex = i;
            break;
        }
    }
    return foundIndex;
}

static const char* convertGen2EventFlagToString(uint16_t eventFlagIndex)
{
    switch(eventFlagIndex)
    {
    case GEN2_EVENTFLAG_DECORATION_PIKACHU_BED:
        return "Pikachu Bed";
    case GEN2_EVENTFLAG_DECORATION_TENTACOOL_DOLL:
        return "Tentacool doll";
    default:
        return "UNKNOWN";
    }
}

void activateFrameLog(void* context, const void*)
{
    MenuScene* scene = static_cast<MenuScene*>(context);
    RDPQGraphics& gfx = scene->getDependencies().gfx;
    debugf("Triggering RDPQ log for 1 frame!\r\n");
    gfx.triggerDebugFrame();
}

void advanceDialog(void* context, const void*)
{
    MenuScene* scene = static_cast<MenuScene*>(context);
    scene->advanceDialog();
}

void goToTestScene(void* context, const void* param)
{
    MenuScene* scene = static_cast<MenuScene*>(context);
    SceneManager& sceneManager = scene->getDependencies().sceneManager;

    sceneManager.switchScene(SceneType::TEST);
}

void goToPokeTransporterGBRef(void* context, const void* param)
{
    MenuScene* scene = static_cast<MenuScene*>(context);
    SceneManager& sceneManager = scene->getDependencies().sceneManager;

    sceneManager.switchScene(SceneType::POKETRANSPORTER_GB_REF);
}

void goToAboutScene(void* context, const void* param)
{
    MenuScene* scene = static_cast<MenuScene*>(context);
    SceneManager& sceneManager = scene->getDependencies().sceneManager;

    sceneManager.switchScene(SceneType::ABOUT);
}

void goToDataCopyScene(void* context, const void* param)
{
    MenuScene* scene = static_cast<MenuScene*>(context);
    const DataCopyOperation operation = (*((const DataCopyOperation*)param));
    SceneManager& sceneManager = scene->getDependencies().sceneManager;

    auto dataCopyContext = new DataCopySceneContext{
        .operation = operation,
        .saveToRestorePath = nullptr
    };

    if(operation == DataCopyOperation::RESTORE_SAVE)
    {
        auto fileSelectContext = new SelectFileSceneContext{
            .titleText = "Select Save file",
            .nextScene = {
                .type = SceneType::COPY_DATA,
                .context = dataCopyContext,
                .deleteContextFunc = deleteDataCopySceneContext
            },
            .fileExtensionFilter = ".sav"
        };
        sceneManager.switchScene(SceneType::SELECT_FILE, deleteSelectFileSceneContext, fileSelectContext);
    }
    else
    {
        sceneManager.switchScene(SceneType::COPY_DATA, deleteDataCopySceneContext, dataCopyContext);
    }
}

void goToGen1DistributionPokemonMenu(void* context, const void*)
{
    goToDistributionPokemonListMenu(context, DistributionPokemonListType::GEN1);
}

void goToGen2DistributionPokemonMenu(void* context, const void*)
{
    goToDistributionPokemonListMenu(context, DistributionPokemonListType::GEN2);
}

void goToGen2PCNYDistributionPokemonMenu(void* context, const void* param)
{
    goToDistributionPokemonListMenu(context, DistributionPokemonListType::GEN2_POKEMON_CENTER_NEW_YORK);
}

void goToGen2DecorationMenu(void* context, const void* param)
{
    MenuScene* scene = static_cast<MenuScene*>(context);
    auto newSceneContext = new MenuSceneContext{
        .menuEntries = gen2DecorationMenuEntries,
        .numMenuEntries = gen2DecorationMenuEntriesSize / sizeof(gen2DecorationMenuEntries[0])
    };

    scene->getDependencies().sceneManager.switchScene(SceneType::MENU, deleteMenuSceneContext, newSceneContext);
}

void goToBackupRestoreMenu(void* context, const void* param)
{
    MenuScene* scene = static_cast<MenuScene*>(context);
    auto newSceneContext = new MenuSceneContext{
        .menuEntries = backupRestoreMenuEntries,
        .numMenuEntries = backupRestoreMenuEntriesSize / sizeof(backupRestoreMenuEntries[0])
    };

    scene->getDependencies().sceneManager.switchScene(SceneType::MENU, deleteMenuSceneContext, newSceneContext); 
}

void gen1PrepareToTeachPikachu(void* context, const void* param)
{
    MenuScene* scene = static_cast<MenuScene*>(context);
    TransferPakManager& tpakManager = scene->getDependencies().tpakManager;
    TransferPakRomReader romReader(tpakManager);
    TransferPakSaveManager saveManager(tpakManager);
    Gen1GameReader gameReader(romReader, saveManager, static_cast<Gen1GameType>(scene->getDependencies().specificGenVersion));
    DialogData* msg1 = nullptr;
    DialogData* msg2 = nullptr;
    uint8_t foundIndex;
    const Move moveType = *static_cast<const Move*>(param);

    tpakManager.setRAMEnabled(true);

    Gen1TrainerPokemon poke;
    Gen1Party party = gameReader.getParty();

    foundIndex = gen1FindPikachuInParty(party);

    if(foundIndex == 0xFF)
    {
        tpakManager.setRAMEnabled(false);
        msg1 = new DialogData{
            .shouldDeleteWhenDone = true
        };
        setDialogDataText(*msg1, "I see you don't have Pikachu in your party. Please come back once you do!");
        scene->showDialog(msg1);
        return;
    }

    if(!party.getPokemon(foundIndex, poke, false))
    {
        tpakManager.setRAMEnabled(false);
        debugf("%s: ERROR while retrieving pokemon from party at index %hu\r\n", __FUNCTION__, foundIndex);
        return;
    }

    tpakManager.setRAMEnabled(false);

    if(poke.index_move1 == (uint8_t)moveType || poke.index_move2 == (uint8_t)moveType || poke.index_move3 == (uint8_t)moveType || poke.index_move4 == (uint8_t)moveType)
    {
        msg1 = new DialogData{
            .shouldDeleteWhenDone = true
        };
        setDialogDataText(*msg1, "Your Pikachu already knows %s!", getMoveString(moveType));
        scene->showDialog(msg1);
        return;
    }

    teachParamsMove1.moveType = teachParamsMove2.moveType = teachParamsMove3.moveType = teachParamsMove4.moveType = moveType;
    teachParamsMove1.partyIndex = teachParamsMove2.partyIndex = teachParamsMove3.partyIndex = teachParamsMove4.partyIndex = foundIndex;
    teachParamsMove1.poke = teachParamsMove2.poke = teachParamsMove3.poke = teachParamsMove4.poke = poke;

    if(!poke.index_move1)
    {
        gen1TeachPikachu(context, &teachParamsMove1);
        return;
    }
    else if(!poke.index_move2)
    {
        gen1TeachPikachu(context, &teachParamsMove2);
        return;
    }
    else if(!poke.index_move3)
    {
        gen1TeachPikachu(context, &teachParamsMove3);
        return;
    }
    else if(!poke.index_move4)
    {
        gen1TeachPikachu(context, &teachParamsMove4);
        return;
    }

    msg2 = new DialogData {
        .options = {
            .items = new MenuItemData[4]{
                {
                    .title = getMoveString(static_cast<Move>(poke.index_move1)),
                    .onConfirmAction = gen1TeachPikachu,
                    .context = context,
                    .itemParam = &teachParamsMove1
                },
                {
                    .title = getMoveString(static_cast<Move>(poke.index_move2)),
                    .onConfirmAction = gen1TeachPikachu,
                    .context = context,
                    .itemParam = &teachParamsMove2
                },
                {
                    .title = getMoveString(static_cast<Move>(poke.index_move3)),
                    .onConfirmAction = gen1TeachPikachu,
                    .context = context,
                    .itemParam = &teachParamsMove3
                },
                {
                    .title = getMoveString(static_cast<Move>(poke.index_move4)),
                    .onConfirmAction = gen1TeachPikachu,
                    .context = context,
                    .itemParam = &teachParamsMove4
                }
            },
            .number = 4,
            .shouldDeleteWhenDone = true
        },
        .shouldDeleteWhenDone = true
    };

    msg1 = new DialogData{
        .next = msg2,
        .shouldDeleteWhenDone = true
    };
    setDialogDataText(*msg1, "Pikachu is trying to learn %s. But Pikachu already knows 4 moves.", getMoveString(moveType));
    setDialogDataText(*msg2, "Delete an older move to make room for %s?", getMoveString(moveType));
    scene->showDialog(msg1);
}

void gen1TeachPikachu(void* context, const void* param)
{
    MenuScene* scene = static_cast<MenuScene*>(context);
    TransferPakManager& tpakManager = scene->getDependencies().tpakManager;
    TransferPakRomReader romReader(tpakManager);
    TransferPakSaveManager saveManager(tpakManager);
    Gen1GameReader gameReader(romReader, saveManager, static_cast<Gen1GameType>(scene->getDependencies().specificGenVersion));

    const Gen1TeachPikachuParams* params = static_cast<const Gen1TeachPikachuParams*>(param);
    Gen1TrainerPokemon poke = params->poke;
    uint8_t previousMove;
    uint8_t newMove = static_cast<uint8_t>(params->moveType);

//  debugf("%s: newMove %hu, moveIndex %hu, partyIndex %hu\r\n", __FUNCTION__, newMove, params->moveIndex, params->partyIndex);

    switch(params->moveIndex)
    {
    case 0:
        previousMove = poke.index_move1;
        poke.index_move1 = newMove;
        poke.pp_move1 = 0;
        break;
    case 1:
        previousMove = poke.index_move2;
        poke.index_move2 = newMove;
        poke.pp_move2 = 0;
        break;
    case 2:
        previousMove = poke.index_move3;
        poke.index_move3 = newMove;
        poke.pp_move3 = 0;
        break;
    case 3:
        previousMove = poke.index_move4;
        poke.index_move4 = newMove;
        poke.pp_move4 = 0;
        break;
    default:
        scene->advanceDialog();
        return;
    }

    tpakManager.setRAMEnabled(true);
    
    Gen1Party party = gameReader.getParty();
    if(!party.setPokemon(params->partyIndex, poke))
    {
        tpakManager.setRAMEnabled(false);
        debugf("%s: ERROR: can't update Pikachu at partyIndex %hu\r\n", __FUNCTION__, params->partyIndex);
        return;
    }
    gameReader.updateMainChecksum();
    tpakManager.finishWrites();
    tpakManager.setRAMEnabled(false);

    DialogData* msg1 = new DialogData{
        .shouldDeleteWhenDone = true
    };

    if(previousMove)
    {
        DialogData* msg3 = new DialogData{
            .shouldDeleteWhenDone = true
        };
        DialogData* msg2 = new DialogData{
            .next = msg3,
            .shouldDeleteWhenDone = true
        };
        msg1->next = msg2;

        setDialogDataText(*msg1, "1..2..3..Poof!");
        setDialogDataText(*msg2, "Pikachu forgot %s!", getMoveString(static_cast<Move>(previousMove)));
        setDialogDataText(*msg3, "Pikachu learned %s!", getMoveString(params->moveType));
    }
    else
    {
        setDialogDataText(*msg1, "Pikachu learned %s!", getMoveString(params->moveType));
    }

    scene->showDialog(msg1);
}

void gen2ReceiveGSBall(void* context, const void* param)
{
    MenuScene* scene = static_cast<MenuScene*>(context);
    TransferPakManager& tpakManager = scene->getDependencies().tpakManager;
    TransferPakRomReader romReader(tpakManager);
    TransferPakSaveManager saveManager(tpakManager);
    Gen2GameReader gameReader(romReader, saveManager, Gen2GameType::CRYSTAL);
    DialogData* messageData = new DialogData{
        .shouldDeleteWhenDone = true
    };

    tpakManager.setRAMEnabled(true);
  
    // the unlockGsBallEvent() function does all the work. It's even repeatable!
    gameReader.unlockGsBallEvent();
    gameReader.finishSave();
    tpakManager.finishWrites();
    tpakManager.setRAMEnabled(false);
    
    setDialogDataText(*messageData, "GS Ball event unlocked! Please go to the Golden Rod Pokémon Center and try to leave!");

    scene->showDialog(messageData);
}

void gen2SetEventFlag(void* context, const void* param)
{
    MenuScene* scene = static_cast<MenuScene*>(context);
    TransferPakManager& tpakManager = scene->getDependencies().tpakManager;
    TransferPakRomReader romReader(tpakManager);
    TransferPakSaveManager saveManager(tpakManager);
    Gen2GameReader gameReader(romReader, saveManager, static_cast<Gen2GameType>(scene->getDependencies().specificGenVersion));
    DialogData* messageData = new DialogData{
        .shouldDeleteWhenDone = true
    };
    const uint16_t eventFlagIndex = *static_cast<const uint16_t*>(param);

    tpakManager.setRAMEnabled(true);

    const char* trainerName = scene->getDependencies().playerName;
    if(gameReader.getEventFlag(eventFlagIndex))
    {
        setDialogDataText(*messageData, "%s already has %s!", trainerName, convertGen2EventFlagToString(eventFlagIndex));
    }
    else
    {
        gameReader.setEventFlag(eventFlagIndex, true);
        gameReader.finishSave();
        tpakManager.finishWrites();

        setDialogDataText(*messageData, "%s has unlocked %s!", trainerName, convertGen2EventFlagToString(eventFlagIndex));
    }

    tpakManager.setRAMEnabled(false);
    scene->showDialog(messageData);
}

void askConfirmationWipeSave(void* context, const void* param)
{
    MenuScene* scene = static_cast<MenuScene*>(context);

    DialogData* messageData = new DialogData{
        .options = {
            .items = new MenuItemData[2]{
                {
                    .title = "Yes",
                    .onConfirmAction = goToDataCopyScene,
                    .context = context,
                    .itemParam = &DATACOPY_WIPE_SAVE
                },
                {
                    .title = "No",
                    .onConfirmAction = advanceDialog,
                    .context = context
                }
            },
            .number = 2,
            .shouldDeleteWhenDone = true
        },
        .shouldDeleteWhenDone = true,
    };

    setDialogDataText(*messageData, "Are you sure you want to wipe the save file from the cartridge?");

    scene->showDialog(messageData);
}

void resetRTC(void* context, const void* param)
{
    // The game checks bit 7 on the sRTCStatusFlags field in SRAM
    // this is set when the game detects wrong RTC register values.
    // In order to let the game prompt to reconfigure the RTC clock, we just have to set this bit
    // Based on sRTCStatusFlags, RecordRTCStatus, .set_bit_7 in
    // https://github.com/pret/pokecrystal
    // https://github.com/pret/pokegold
    const uint8_t rtcStatusFieldValue = 0xC0;
    MenuScene* scene = static_cast<MenuScene*>(context);

    auto diag = new DialogData{
        .shouldDeleteWhenDone = true
    };

    if(scene->getDependencies().generation != 2)
    {
        setDialogDataText(*diag, "Sorry! This is only supported for Gen 2 Pokémon games!");
        scene->showDialog(diag);
        return;
    }

    TransferPakManager& tpakManager = scene->getDependencies().tpakManager;

    tpakManager.setRAMEnabled(true);

    tpakManager.switchGBSRAMBank(0);

    tpakManager.writeSRAM(0xC60, &rtcStatusFieldValue, 1);
    tpakManager.finishWrites();

    setDialogDataText(*diag, "The games' clock was reset! Start the game to reconfigure it! Don't forget to save!");
    scene->showDialog(diag);
}