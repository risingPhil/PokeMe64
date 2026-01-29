#include "scenes/PokeShopScene.h"
#include "scenes/SceneManager.h"
#include "scenes/StatsScene.h"
#include "transferpak/TransferPakManager.h"
#include "menu/PokeShopEntries.h"

static const Rectangle menuListBounds = {20, 20, 280, 0};
static const Rectangle imgScrollArrowUpBounds = {.x = 154, .y = 14, .width = 11, .height = 6};
static const Rectangle imgScrollArrowDownBounds = {.x = 154, .y = 220, .width = 11, .height = 6};

static void injectShopPokemon(void* context, const void* data)
{
    auto scene = static_cast<PokeShopScene*>(context);
    scene->triggerPokemonInjection(data);
}

PokeShopScene::PokeShopScene(SceneDependencies& deps, void* context)
    : MenuScene(deps, context)
    , romReader_(deps.tpakManager)
    , saveManager_(deps.tpakManager)
    , gen1Reader_(romReader_, saveManager_, static_cast<Gen1GameType>(deps.specificGenVersion), static_cast<Gen1LocalizationLanguage>(deps.localization))
    , gen2Reader_(romReader_, saveManager_, static_cast<Gen2GameType>(deps.specificGenVersion), static_cast<Gen2LocalizationLanguage>(deps.localization))
    , iconFactory_(romReader_)
    , customListFiller_(menuList_)
    , diag_()
    , iconBackgroundSprite_(nullptr)
    , pokeToInject_(nullptr)
{
}

PokeShopScene::~PokeShopScene()
{
}

void PokeShopScene::init()
{
    iconBackgroundSprite_ = sprite_load("rom://bg-party-icon.sprite");
    loadShopList();
    MenuScene::init();
}

void PokeShopScene::destroy()
{
    MenuScene::destroy();

    delete[] context_->menuEntries;
    context_->menuEntries = nullptr;
    context_->numMenuEntries = 0;

    sprite_free(iconBackgroundSprite_);
    iconBackgroundSprite_ = nullptr;
}

bool PokeShopScene::handleUserInput(joypad_port_t port, const joypad_inputs_t& inputs)
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

void PokeShopScene::triggerPokemonInjection(const void* data)
{
    pokeToInject_ = data;

    setDialogDataText(diag_, "Saving... Don't turn off the power.");
    diag_.userAdvanceBlocked = true;
    showDialog(&diag_);
}

void PokeShopScene::injectPokemon(const void* data)
{
#if 0
    StatsSceneContext* statsContext;
    const Gen1DistributionPokemon* g1Poke;
    const Gen2DistributionPokemon* g2Poke;
    const char* trainerName;

    deps_.tpakManager.setRAMEnabled(true);

    statsContext = new StatsSceneContext{
        .showReceivedPokemonDialog = true
    };

    switch(convert(context_)->listType)
    {
    case DistributionPokemonListType::GEN1:
        g1Poke = static_cast<const Gen1DistributionPokemon*>(data);
        statsContext->poke_g1 = g1Poke->poke;
        // I have not used gen1Reader_->addDistributionPokemon here because I want to show the resulting pokemon in a stats screen
        // gen1_prepareDistributionPokemon() + addPokemon() gives me access to the resulting Gen1TrainerPokemon instance
        // in which things are done like IV generation, OT name decision, OT id
        gen1_prepareDistributionPokemon(gen1Reader_, (*g1Poke), statsContext->poke_g1, trainerName);
        gen1Reader_.addPokemon(statsContext->poke_g1, trainerName);
        break;
    case DistributionPokemonListType::GEN2:
    case DistributionPokemonListType::GEN2_POKEMON_CENTER_NEW_YORK:
        g2Poke = static_cast<const Gen2DistributionPokemon*>(data);
        statsContext->poke_g2 = g2Poke->poke;
        statsContext->isEgg = g2Poke->isEgg;
        // I have not used gen2Reader_->addDistributionPokemon here because I want to show the resulting pokemon in a stats screen
        // gen2_prepareDistributionPokemon() + addPokemon() gives me access to the resulting Gen2TrainerPokemon instance
        // in which things are done like IV generation, OT name decision, OT id, shininess
        gen2_prepareDistributionPokemon(gen2Reader_, (*g2Poke), statsContext->poke_g2, trainerName);
        gen2Reader_.addPokemon(statsContext->poke_g2, g2Poke->isEgg, trainerName);
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
        delete statsContext;
        statsContext = nullptr;
        return;
    }

    deps_.tpakManager.finishWrites();

    // The reason is the same as previous setRAMEnabled(false) statement above
    deps_.tpakManager.setRAMEnabled(false);

    strncpy(statsContext->trainerName, trainerName, 12);
    deps_.sceneManager.switchScene(SceneType::STATS, deleteStatsSceneContext, statsContext);

    // operation done. Now the dialog can be advanced and we can show confirmation that the user got the pokémon
#endif
}

void PokeShopScene::onDialogDone()
{
    if(diag_.userAdvanceBlocked)
    {
        // ignore this notification. We advanced this one ourselves to get to the next one
        return;
    }
    // We're done with the injection. Go back to the previous menu
    deps_.sceneManager.goBackToPreviousScene();
}

void PokeShopScene::setupMenu()
{
    const VerticalListStyle listStyle = {
        .margin = {
            .top = 5,
            .bottom = 5
        },
        .verticalSpacingBetweenWidgets = 1,
        .autogrow = {
            .enabled = true,
            .maxHeight = 200
        }
    };

    menuList_.setStyle(listStyle);
    menuList_.setBounds(menuListBounds);
    menuList_.setVisible(true);
    menuList_.registerScrollWindowListener(this);

    cursorWidget_.setVisible(false);

    const PokeShopMenuItemStyle itemStyle = {
        .size = {280, 22},
        .background = {
            .sprite = menu9SliceSprite_,
            .spriteSettings = {
                .renderMode = SpriteRenderMode::NINESLICE,
                .srcRect = { 6, 6, 6, 6 }
            }
        },
        .icon = {
            .style = {
                .background = {
                    .sprite = iconBackgroundSprite_
                },
                .icon = {
                    .bounds = { 2, 2, 16, 16 },
                    .yOffsetWhenTheresNoFrame2 = -1
                },
                .fpsWhenFocused = 8,
                .fpsWhenNotFocused = 2
            },
            .bounds = {0, 1, 20, 20}
        },
        .title = {
            .labelNotFocused = {
                .fontId = mainFontId_,
                .fontStyleId = fontStyleWhiteId_
            },
            .labelFocused = {
                .fontId = mainFontId_,
                .fontStyleId = fontStyleYellowId_
            },
            .bounds = {24, 4, 100, 18}
        },
        .price = {
            .labelNotFocused = {
                .fontId = mainFontId_,
                .fontStyleId = fontStyleWhiteId_
            },
            .labelFocused = {
                .fontId = mainFontId_,
                .fontStyleId = fontStyleYellowId_
            },
            .bounds = {104, 4, 80, 18}
        }
    };

    customListFiller_.addItems(static_cast<PokeShopMenuItemData*>(context_->menuEntries), context_->numMenuEntries, itemStyle);

    const ImageWidgetStyle scrollArrowUpStyle = {
        .image = {
            .sprite = uiArrowUpSprite_,
            .spriteBounds = {0, 0, imgScrollArrowUpBounds.width, imgScrollArrowUpBounds.height}
        }
    };

    scrollArrowUp_.setStyle(scrollArrowUpStyle);
    scrollArrowUp_.setBounds(imgScrollArrowUpBounds);

    const ImageWidgetStyle scrollArrowDownStyle = {
        .image = {
            .sprite = uiArrowDownSprite_,
            .spriteBounds = { 0, 0, imgScrollArrowDownBounds.width, imgScrollArrowDownBounds.height}
        }
    };

    // note: even though autogrow is turned on for the vertical list, it doesn't matter for the down arrow.
    // because when the list is still growing, no scrolling is needed anyway, so the arrow would be invisible anyway.
    scrollArrowDown_.setStyle(scrollArrowDownStyle);
    scrollArrowDown_.setBounds(imgScrollArrowDownBounds);
}

void PokeShopScene::loadShopList()
{
    MenuSceneContext *context = (MenuSceneContext*)context_;
    const Gen1PokeShopEntry *gen1shopEntries = nullptr;
    const Gen2PokeShopEntry *gen2shopEntries = nullptr;
    uint32_t listSize = 0;
    uint32_t i;
    uint8_t iconType;

    switch(deps_.generation)
    {
    case 1:
        getGen1PokeShopEntries(deps_.specificGenVersion, deps_.localization, gen1shopEntries, listSize);
        break;
    case 2:
        getGen2PokeShopEntries(deps_.specificGenVersion, deps_.localization, gen2shopEntries, listSize);
        break;
    default:
        return;
    }

    if(!listSize)
    {
        return;
    }
    context->menuEntries = new PokeShopMenuItemData[listSize];
    context->numMenuEntries = listSize;

    for(i = 0; i < listSize; ++i)
    {
        PokeShopMenuItemData* menuEntry =  static_cast<PokeShopMenuItemData*>(context_->menuEntries) + i;
        
        if(deps_.generation == 1)
        {
            iconType = (uint8_t)gen1Reader_.getPokemonIconType(gen1shopEntries[i].pokemon->poke.poke_index);

            menuEntry->title = gen1shopEntries[i].pokemon->name;
            menuEntry->itemParam = &(gen1shopEntries[i]);
            menuEntry->price = gen1shopEntries[i].price;
        }
        else
        {
            if(gen2shopEntries[i].pokemon->isEgg)
            {
                iconType = (uint8_t)Gen2PokemonIconType::GEN2_ICONTYPE_EGG;
            }
            else
            {
                iconType = (uint8_t)gen2Reader_.getPokemonIconType(gen2shopEntries[i].pokemon->poke.poke_index);
            }

            menuEntry->title = gen2shopEntries[i].pokemon->name;
            menuEntry->itemParam = &(gen2shopEntries[i]);
            menuEntry->price = gen2shopEntries[i].price;
        }

        menuEntry->onConfirmAction = injectShopPokemon;
        menuEntry->context = this;
        menuEntry->iconData = {
            .iconFactory = &iconFactory_,
            .generation = deps_.generation,
            .specificGenVersion = deps_.specificGenVersion,
            .localization = deps_.localization,
            .iconType = iconType
        };
    }
}