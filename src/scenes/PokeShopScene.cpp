#include "scenes/PokeShopScene.h"
#include "scenes/SceneManager.h"
#include "scenes/StatsScene.h"
#include "transferpak/TransferPakManager.h"
#include "menu/PokeShopEntries.h"
#include "menu/MenuFunctions.h"

#include <cstdlib>

static const Rectangle menuListBounds = {165, 40, 130, 0};
static const Rectangle imgScrollArrowUpBounds = {.x = 235, .y = 40, .width = 11, .height = 6};
static const Rectangle imgScrollArrowDownBounds = {.x = 235, .y = 170, .width = 11, .height = 6};

static const char* const shopkeeperDialogTexts[] = {
    "Fell of a truck, that one. Poor guy.",
    "Bruises? These just love fighting, man.",
    "Don't worry about it. It's just a rash!",
    "Don't mind the black eye. Just bumped its head this morning.",
    "Real clumsy, this one. Gets cuts almost daily!",
    "This one's a real champ! But champs get bruises, ya know.",
    "Welts? Nah man, just birth marks.",
    "It was like this when we found it.",
    "A trainer tag? Nah, it just likes cosplaying.",
    "Stolen? Nah man, the original trainer just dumped it.",
    "Those lumps? This idiot kept slamming its head yesterday.",
    "Just keep it away from authorities. It's real scared of them!",
    "Try to avoid showing this one around. People might -eh- get jealous.",
    "Yeah it's expensive, but this one was a pain to ..erm.. acquire.",
    "Don't tell anyone where you got it. We want to keep it exclusive, ya know.",
    "I like you, so I'll let you buy it. But don't go bragging about it, alright?"
};

static const char* getRandomShopKeeperText()
{
    size_t numEntries = sizeof(shopkeeperDialogTexts) / sizeof(const char*);
    const int index = rand() % static_cast<int>(numEntries);
    return shopkeeperDialogTexts[index];
}

static void triggerPurchaseCallback(void *context, const void* data)
{
    auto scene = static_cast<PokeShopScene*>(context);
    scene->triggerCompletePurchase(data);
}

static void promptBuyActionCallback(void* context, const void* data)
{    
    DialogData *diag = new DialogData{
        .options = {
            .items = new MenuItemData[2] {
                {
                    .title = "Buy",
                    .onConfirmAction = triggerPurchaseCallback,
                    .context = context,
                    .itemParam = data
                },
                {
                    .title = "Cancel",
                    .onConfirmAction = advanceDialog,
                    .context = context
                }
            },
            .number = 2,
            .shouldDeleteWhenDone = true
        },
        .next = nullptr,
        .shouldDeleteWhenDone = true,
        .userAdvanceBlocked = false
    };

    auto scene = static_cast<PokeShopScene*>(context);
    setDialogDataText(*diag, getRandomShopKeeperText());
    scene->showDialog(diag);
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
    , backgroundImgSprite_(nullptr)
    , iconBackgroundSprite_(nullptr)
    , priceBorderSprite_(nullptr)
    , pokeToInject_(nullptr)
{
}

PokeShopScene::~PokeShopScene()
{
}

void PokeShopScene::init()
{
    backgroundImgSprite_ = sprite_load("rom://rocketshop.sprite");
    iconBackgroundSprite_ = sprite_load("rom://bg-party-icon.sprite");
    priceBorderSprite_ = sprite_load("rom://border-bg-9slice.sprite");
    loadShopList();
    MenuScene::init();
}

void PokeShopScene::destroy()
{
    MenuScene::destroy();

    delete[] context_->menuEntries;
    context_->menuEntries = nullptr;
    context_->numMenuEntries = 0;

    sprite_free(priceBorderSprite_);
    priceBorderSprite_ = nullptr;
    sprite_free(iconBackgroundSprite_);
    iconBackgroundSprite_ = nullptr;
    sprite_free(backgroundImgSprite_);
    backgroundImgSprite_ = nullptr;
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

void PokeShopScene::render(RDPQGraphics& gfx, const Rectangle& sceneBounds)
{
    // draw the background image
    if(backgroundImgSprite_)
    {
        const SpriteRenderSettings renderSettings = {
            .renderMode = SpriteRenderMode::NORMAL
        };
        gfx.drawSprite({0, 0, backgroundImgSprite_->width, backgroundImgSprite_->height}, backgroundImgSprite_, renderSettings);
    }

    MenuScene::render(gfx, sceneBounds);
}

void PokeShopScene::triggerCompletePurchase(const void* data)
{
    uint32_t trainerMoney;
    uint32_t moneyRequired;

    deps_.tpakManager.setRAMEnabled(true);
    switch(deps_.generation)
    {
    case 1:
        trainerMoney = gen1Reader_.getTrainerMoney();
        moneyRequired = static_cast<const Gen1PokeShopEntry*>(data)->price;
        break;
    case 2:
        trainerMoney = gen2Reader_.getTrainerMoney();
        moneyRequired = static_cast<const Gen2PokeShopEntry*>(data)->price;
        break;
    default:
        trainerMoney = 0;
        moneyRequired = 0xFFFFFFFF;
        break;
    }
    deps_.tpakManager.setRAMEnabled(false);

    if(trainerMoney >= moneyRequired)
    {
        pokeToInject_ = data;
        setDialogDataText(diag_, "Saving... Don't turn off the power.");
        diag_.userAdvanceBlocked = true;
    }
    else
    {
        setDialogDataText(diag_, "Can't afford it, bud!");
        diag_.userAdvanceBlocked = false;        
    }

    showDialog(&diag_);
}

void PokeShopScene::injectPokemon(const void* data)
{
    const Gen1PokeShopEntry* g1Entry;
    const Gen2PokeShopEntry* g2Entry;
    const char* trainerName;
    const char* pokeName;

    deps_.tpakManager.setRAMEnabled(true);

    switch(deps_.generation)
    {
    case 1:
        g1Entry = static_cast<const Gen1PokeShopEntry*>(data);
        trainerName = gen1Reader_.getTrainerName();
        pokeName = g1Entry->pokemon->name;
        gen1Reader_.setTrainerMoney(gen1Reader_.getTrainerMoney() - g1Entry->price);
        gen1Reader_.addDistributionPokemon(*g1Entry->pokemon);
        break;
    case 2:
        g2Entry = static_cast<const Gen2PokeShopEntry*>(data);
        trainerName = gen2Reader_.getTrainerName();
        pokeName = g2Entry->pokemon->name;
        gen2Reader_.setTrainerMoney(gen2Reader_.getTrainerMoney() - g2Entry->price);
        gen2Reader_.addDistributionPokemon(*g2Entry->pokemon);
        gen2Reader_.finishSave();
        break;
    default:
        trainerName = "Dummy";
        pokeName = trainerName;
        debugf("%s: ERROR: Invalid gen %hu This should never happen!\r\n", __FUNCTION__, deps_.generation);
        break;
    }

    deps_.tpakManager.finishWrites();

    // The reason is the same as previous setRAMEnabled(false) statement above
    deps_.tpakManager.setRAMEnabled(false);

    diag_.userAdvanceBlocked = false;
    setDialogDataText(diag_, "%s got a %s!", trainerName, pokeName);
    showDialog(&diag_);
}

void PokeShopScene::onDialogDone()
{
    MenuScene::onDialogDone();
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
            .maxHeight = 130
        }
    };

    menuList_.setStyle(listStyle);
    menuList_.setBounds(menuListBounds);
    menuList_.setVisible(true);
    menuList_.registerScrollWindowListener(this);

    cursorWidget_.setVisible(false);

    const PokeShopMenuItemStyle itemStyle = {
        .size = {150, 22},
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
            .bounds = {100, 4, 80, 18}
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

        menuEntry->onConfirmAction = promptBuyActionCallback;
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