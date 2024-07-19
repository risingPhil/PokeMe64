#include "menu/MenuFunctions.h"
#include "menu/MenuEntries.h"
#include "core/RDPQGraphics.h"
#include "scenes/DistributionPokemonListScene.h"
#include "scenes/SceneManager.h"
#include "gen2/Gen2GameReader.h"
#include "transferpak/TransferPakManager.h"
#include "transferpak/TransferPakRomReader.h"
#include "transferpak/TransferPakSaveManager.h"

#define POKEMON_CRYSTAL_ITEM_ID_GS_BALL 0x73

#if 0
static void goToMenu(void* context, MenuItemData* menuEntries, uint32_t numMenuEntries)
{
    MenuScene* scene = static_cast<MenuScene*>(context);
    SceneManager& sceneManager = scene->getDependencies().sceneManager;

    MenuSceneContext* sceneContext = new MenuSceneContext{
        .menuEntries = menuEntries,
        .numMenuEntries = numMenuEntries
    };

    sceneManager.switchScene(SceneType::MENU, deleteMenuSceneContext, sceneContext);
}
#endif

static void goToDistributionPokemonListMenu(void* context, DistributionPokemonListType type)
{
    auto sceneContext = new DistributionPokemonListSceneContext;
    sceneContext->listType = type;

    MenuScene* scene = static_cast<MenuScene*>(context);
    SceneManager& sceneManager = scene->getDependencies().sceneManager;

    sceneManager.switchScene(SceneType::DISTRIBUTION_POKEMON_LIST, deleteDistributionPokemonListSceneContext, sceneContext);
}

void printMessage(void* context, const void*)
{
    debugf((const char*)context);
}

void activateFrameLog(void* context, const void*)
{
    MenuScene* scene = static_cast<MenuScene*>(context);
    RDPQGraphics& gfx = scene->getDependencies().gfx;
    debugf("Triggering RDPQ log for 1 frame!\r\n");
    gfx.triggerDebugFrame();
}

void goToTestScene(void* context, const void* param)
{
    MenuScene* scene = static_cast<MenuScene*>(context);
    SceneManager& sceneManager = scene->getDependencies().sceneManager;

    sceneManager.switchScene(SceneType::TEST);
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

void gen2ReceiveGSBall(void* context, const void* param)
{
    MenuScene* scene = static_cast<MenuScene*>(context);
    TransferPakManager& tpakManager = scene->getDependencies().tpakManager;
    TransferPakRomReader romReader(tpakManager);
    TransferPakSaveManager saveManager(tpakManager);
    Gen2GameReader gameReader(romReader, saveManager, Gen2GameType::CRYSTAL);
    DialogData messageData = {0};
    bool alreadyHasOne = false;

    tpakManager.setRAMEnabled(true);

    const char* trainerName = gameReader.getTrainerName();
    Gen2ItemList keyItemPocket = gameReader.getItemList(Gen2ItemListType::GEN2_ITEMLISTTYPE_KEYITEMPOCKET);
    if(keyItemPocket.getCount() > 0)
    {
        uint8_t itemId;
        uint8_t itemCount;
        bool gotEntry = keyItemPocket.getEntry(0, itemId, itemCount);

        while(gotEntry)
        {
            if(itemId == POKEMON_CRYSTAL_ITEM_ID_GS_BALL)
            {
                alreadyHasOne = true;
                break;
            }
            gotEntry = keyItemPocket.getNextEntry(itemId, itemCount);
        }
    }
    
    if(alreadyHasOne)
    {
        setDialogDataText(messageData, "It appears you already have one!");
    }
    else
    {
        keyItemPocket.add(POKEMON_CRYSTAL_ITEM_ID_GS_BALL, 1);
        gameReader.finishSave();
        setDialogDataText(messageData, "%s obtained a GS Ball!", trainerName);
    }
    scene->showSingleMessage(messageData);

    tpakManager.setRAMEnabled(false);
}