#include "scenes/DistributionPokemonListScene.h"
#include "scenes/SceneManager.h"
#include "transferpak/TransferPakManager.h"

static uint8_t calculateMainDataChecksum(ISaveManager& saveManager)
{
    Gen1Checksum checksum;
    const uint16_t checksummedDataStart = 0x598;
    const uint16_t checksummedDataEnd = 0x1523;
    const uint16_t numBytes = checksummedDataEnd - checksummedDataStart;
    uint16_t i;
    uint8_t temp = 0;
    uint8_t byte;

    saveManager.seekToBankOffset(1, checksummedDataStart);

    debugf("Checksum - dumping bytes:\r\n");
    for(i=0; i < numBytes; ++i)
    {
        saveManager.readByte(byte);
        debugf(" %02x", byte);
        checksum.addByte(byte);
        temp += byte;
    }

    uint8_t ret = checksum.get();
    debugf("\r\ntemp=%02x, ret=%02x\r\n", temp, ret);
    return ret;
}

static DistributionPokemonListSceneContext* convert(void* context)
{
    return static_cast<DistributionPokemonListSceneContext*>(context);
}

static void injectDistributionPokemon(void* context, const void* data)
{
    auto scene = static_cast<DistributionPokemonListScene*>(context);
    scene->triggerPokemonInjection(data);
}

DistributionPokemonListScene::DistributionPokemonListScene(SceneDependencies& deps, void* context)
    : MenuScene(deps, context)
    , romReader_(deps.tpakManager)
    , saveManager_(deps.tpakManager)
    , gen1Reader_(romReader_, saveManager_, static_cast<Gen1GameType>(deps.specificGenVersion))
    , gen2Reader_(romReader_, saveManager_, static_cast<Gen2GameType>(deps.specificGenVersion))
    , diag_()
    , pokeToInject_(nullptr)
{
}

DistributionPokemonListScene::~DistributionPokemonListScene()
{
}

void DistributionPokemonListScene::init()
{
    loadDistributionPokemonList();
    MenuScene::init();
}

void DistributionPokemonListScene::destroy()
{
    MenuScene::destroy();

    delete[] context_->menuEntries;
    context_->menuEntries = nullptr;
    context_->numMenuEntries = 0;
}

bool DistributionPokemonListScene::handleUserInput(joypad_port_t port, const joypad_inputs_t& inputs)
{
    if(pokeToInject_)
    {
        injectPokemon(pokeToInject_);
        pokeToInject_ = nullptr;
        return true;
    }
    else
    {
        return MenuScene::handleUserInput(port, inputs);
    }
}

void DistributionPokemonListScene::triggerPokemonInjection(const void* data)
{
    pokeToInject_ = data;

    setDialogDataText(diag_, "Saving... Don't turn off the power.");
    diag_.userAdvanceBlocked = true;
    showDialog(&diag_);
}

void DistributionPokemonListScene::injectPokemon(const void* data)
{
    const Gen1DistributionPokemon* g1Poke;
    const Gen2DistributionPokemon* g2Poke;
    const char* trainerName;
    const char* pokeName;

    deps_.tpakManager.setRAMEnabled(true);

    switch(convert(context_)->listType)
    {
    case DistributionPokemonListType::GEN1:
        g1Poke = static_cast<const Gen1DistributionPokemon*>(data);
        trainerName = gen1Reader_.getTrainerName();
        pokeName = gen1Reader_.getPokemonName(g1Poke->poke.poke_index);
        gen1Reader_.addDistributionPokemon((*g1Poke));
        break;
    case DistributionPokemonListType::GEN2:
    case DistributionPokemonListType::GEN2_POKEMON_CENTER_NEW_YORK:
        g2Poke = static_cast<const Gen2DistributionPokemon*>(data);
        trainerName = gen2Reader_.getTrainerName();
        pokeName = gen2Reader_.getPokemonName(g2Poke->poke.poke_index);
        gen2Reader_.addDistributionPokemon((*g2Poke));
        gen2Reader_.finishSave();
        break;
    default:
        debugf("%s: ERROR: got DistributionPokemonListType::INVALID! This should never happen!\r\n", __FUNCTION__);
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
        return;
    }

    deps_.tpakManager.finishWrites();

    calculateMainDataChecksum(saveManager_);

    // The reason is the same as previous setRAMEnabled(false) statement above
    deps_.tpakManager.setRAMEnabled(false);

    // operation done. Now the dialog can be advanced and we can show confirmation that the user got the pokémon
    dialogWidget_.advanceDialog();

    setDialogDataText(diag_, "%s received %s!", trainerName, pokeName);
    diag_.userAdvanceBlocked = false;
    showDialog(&diag_);
}

void DistributionPokemonListScene::onDialogDone()
{
    if(diag_.userAdvanceBlocked)
    {
        // ignore this notification. We advanced this one ourselves to get to the next one
        return;
    }
    // We're done with the injection. Go back to the previous menu
    deps_.sceneManager.goBackToPreviousScene();
}

void DistributionPokemonListScene::setupMenu()
{
    const VerticalListStyle listStyle = {
        .backgroundSprite = menu9SliceSprite_,
        .backgroundSpriteSettings = {
            .renderMode = SpriteRenderMode::NINESLICE,
            .srcRect = { 6, 6, 6, 6 }
        },
        .marginTop = 5,
    };

    menuList_.setStyle(listStyle);
    menuList_.setBounds(Rectangle{20, 20, 280, 150});
    menuList_.setVisible(true);

    cursorWidget_.setVisible(false);

    const MenuItemStyle itemStyle = {
        .size = {280, 16},
        .titleNotFocused = {
            .fontId = arialId_,
            .fontStyleId = fontStyleWhiteId_
        },
        .titleFocused = {
            .fontId = arialId_,
            .fontStyleId = fontStyleYellowId_
        },
        .leftMargin = 10,
        .topMargin = 1
    };

    menuListFiller_.addItems(context_->menuEntries, context_->numMenuEntries, itemStyle);
}

void DistributionPokemonListScene::loadDistributionPokemonList()
{
    const Gen1DistributionPokemon** gen1List;
    const Gen2DistributionPokemon** gen2List;
    uint32_t listSize;
    uint32_t i;

    DistributionPokemonListSceneContext* context = convert(context_);

    switch(context->listType)
    {
    case DistributionPokemonListType::GEN1:
        gen1_getMainDistributionPokemonList(gen1List, listSize);
        gen2List = nullptr;
        break;
    case DistributionPokemonListType::GEN2:
        gen1List = nullptr;
        gen2_getMainDistributionPokemonList(gen2List, listSize);
        break;
    case DistributionPokemonListType::GEN2_POKEMON_CENTER_NEW_YORK:
        gen1List = nullptr;
        gen2_getPokemonCenterNewYorkDistributionPokemonList(gen2List, listSize);
        break;
    default:
        gen1List = nullptr;
        gen2List = nullptr;
        listSize = 0;
        break;
    }

    if(!listSize)
    {
        return;
    }
    context->menuEntries = new MenuItemData[listSize];
    context->numMenuEntries = listSize;

    if(gen1List)
    {
        for(i = 0; i < listSize; ++i)
        {
            context->menuEntries[i].title = gen1List[i]->name;
            context->menuEntries[i].onConfirmAction = injectDistributionPokemon;
            context->menuEntries[i].context = this;
            context->menuEntries[i].itemParam = gen1List[i];
        }
    }
    else if(gen2List)
    {
        for(i = 0; i < listSize; ++i)
        {
            context->menuEntries[i].title = gen2List[i]->name;
            context->menuEntries[i].onConfirmAction = injectDistributionPokemon;
            context->menuEntries[i].context = this;
            context->menuEntries[i].itemParam = gen2List[i];
        }
    }
}

void deleteDistributionPokemonListSceneContext(void* context)
{
    auto toDelete = static_cast<DistributionPokemonListSceneContext*>(context);
    delete toDelete;
}