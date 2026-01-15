#include "scenes/PokeShopScene.h"
#include "scenes/SceneManager.h"
#include "scenes/StatsScene.h"
#include "transferpak/TransferPakManager.h"

static const Rectangle menuListBounds = {20, 20, 280, 0};
static const Rectangle imgScrollArrowUpBounds = {.x = 154, .y = 14, .width = 11, .height = 6};
static const Rectangle imgScrollArrowDownBounds = {.x = 154, .y = 220, .width = 11, .height = 6};

static void injectDistributionPokemon(void* context, const void* data)
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