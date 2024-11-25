#include "scenes/StatsScene.h"
#include "SpriteRenderer.h"
#include "core/FontManager.h"
#include "scenes/SceneManager.h"
#include "transferpak/TransferPakManager.h"
#include "Moves.h"

using OutputFormat = SpriteRenderer::OutputFormat;

static const Rectangle backgroundBounds = {30, 25, 260, 140};
static const Rectangle spriteBounds = {37, 32, 0, 0};
static const Rectangle nameBounds = {90, 37, 150, 16};
static const Rectangle levelAndNumberBounds = {90, 53, 100, 32};
static const Rectangle otInfoBounds = {185, 53, 100, 32};
static const Rectangle statsBounds = {42, 96, 150, 80};
static const Rectangle movesBounds = {185, 96, 150, 80};

static uint8_t getNumBytesPerColorFor(OutputFormat format)
{
    switch(format)
    {
        case OutputFormat::RGB:
            return 3;
        case OutputFormat::RGBA16:
            return 2;
        case OutputFormat::RGBA32:
            return 4;
        default:
            return 0;
    }
}

static Gen1GameType getGen1TypeFor(uint8_t generation, uint8_t specificGenVersion)
{
    if(generation != 1)
    {
        return Gen1GameType::INVALID;
    }
    return static_cast<Gen1GameType>(specificGenVersion);
}

static Gen2GameType getGen2TypeFor(uint8_t generation, uint8_t specificGenVersion)
{
    if(generation != 2)
    {
        return Gen2GameType::INVALID;
    }
    return static_cast<Gen2GameType>(specificGenVersion);
}

static void dialogFinishedCallback(void* context)
{
    StatsScene* scene = (StatsScene*)context;
    scene->onDialogFinished();
}

StatsScene::StatsScene(SceneDependencies& deps, void* context)
    : SceneWithDialogWidget(deps)
    , diag_({0})
    , romReader_(deps.tpakManager)
    , saveManager_(deps.tpakManager)
    , gen1GameReader_(romReader_, saveManager_, getGen1TypeFor(deps.generation, deps.specificGenVersion), static_cast<Gen1LocalizationLanguage>(deps.localization))
    , gen2GameReader_(romReader_, saveManager_, getGen2TypeFor(deps.generation, deps.specificGenVersion), static_cast<Gen2LocalizationLanguage>(deps.localization))
    , menu9SliceSprite_(nullptr)
    , backgroundRenderSettings_({
            .renderMode = SpriteRenderMode::NINESLICE,
            .srcRect = Rectangle{6, 6, 6, 6}
        })
    , fontArialSmallId_(2)
    , fontArialSmallWhiteId_(0)
    , textSettings_(TextRenderSettings{
        .fontId = arialId_,
        .fontStyleId = fontStyleWhiteId_
    })
    , smallTextSettings_(TextRenderSettings{
        .fontId = fontArialSmallId_,
        .fontStyleId = fontArialSmallWhiteId_
    })
    , statsSettings_(TextRenderSettings{
        .fontId = arialId_,
        .fontStyleId = fontStyleWhiteId_,
    })
    , spriteBounds_(spriteBounds)
    , nameBuffer_()
    , levelAndNumberBuffer_()
    , pokeStatsString_()
    , otInfoString_()
    , context_(static_cast<StatsSceneContext*>(context))
    , pokeSpriteSurface_({0})
{
    nameBuffer_[0] = '\0';
    nameBuffer_[sizeof(nameBuffer_) - 1] = '\0';
}

StatsScene::~StatsScene()
{
}

void StatsScene::init()
{
    uint8_t pokeIndex;
    uint8_t level;
    uint8_t pokeNumber;
    uint16_t hp;
    uint16_t trainerID;
    uint16_t atk;
    uint16_t def;
    uint16_t specAtk;
    uint16_t specDef;
    uint16_t speed;
    const char* move1Str;
    const char* move2Str;
    const char* move3Str;
    const char* move4Str;
    const char* pokeName;
    const char* trainerName;
    const char* shinyText;
    bool shiny;

    menu9SliceSprite_ = sprite_load("rom://menu-bg-9slice.sprite");
    fontArialSmallId_ = deps_.fontManager.getFont("rom://Arial-small.font64");
    trainerName = deps_.playerName;

    SceneWithDialogWidget::init();

    const rdpq_fontstyle_t arialWhite = {
        .color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF),
        .outline_color = RGBA32(0, 0, 0, 0xFF)
    };
    deps_.fontManager.registerFontStyle(fontArialSmallId_, fontArialSmallWhiteId_, arialWhite);
    deps_.tpakManager.setRAMEnabled(true);
    switch(deps_.generation)
    {
        case 1:
            gen1_recalculatePokeStats(gen1GameReader_, context_->poke_g1);
            pokeIndex = context_->poke_g1.poke_index;
            level = context_->poke_g1.level;
            pokeNumber = gen1GameReader_.getPokemonNumber(pokeIndex);
            hp = context_->poke_g1.max_hp;
            trainerID = context_->poke_g1.original_trainer_ID;
            atk = context_->poke_g1.atk;
            def = context_->poke_g1.def;
            specAtk = context_->poke_g1.special;
            speed = context_->poke_g1.speed;
            move1Str = getMoveString(static_cast<Move>(context_->poke_g1.index_move1));
            move2Str = getMoveString(static_cast<Move>(context_->poke_g1.index_move2));
            move3Str = getMoveString(static_cast<Move>(context_->poke_g1.index_move3));
            move4Str = getMoveString(static_cast<Move>(context_->poke_g1.index_move4));
            pokeName = gen1GameReader_.getPokemonName(pokeIndex);
            shiny = false;
            snprintf(pokeStatsString_, sizeof(pokeStatsString_), "ATK:            %u\nDEF:            %u\nSPEC:          %u\nSPEED:        %u", atk, def, specAtk, speed);
            break;
        case 2:
            gen2_recalculatePokeStats(gen2GameReader_, context_->poke_g2);
            pokeIndex = context_->poke_g2.poke_index;
            level = context_->poke_g2.level;
            pokeNumber = pokeIndex;
            hp = context_->poke_g2.max_hp;
            trainerID = context_->poke_g2.original_trainer_ID;
            atk = context_->poke_g2.atk;
            def = context_->poke_g2.def;
            specAtk = context_->poke_g2.special_atk;
            specDef = context_->poke_g2.special_def;
            speed = context_->poke_g2.speed;
            move1Str = getMoveString(static_cast<Move>(context_->poke_g2.index_move1));
            move2Str = getMoveString(static_cast<Move>(context_->poke_g2.index_move2));
            move3Str = getMoveString(static_cast<Move>(context_->poke_g2.index_move3));
            move4Str = getMoveString(static_cast<Move>(context_->poke_g2.index_move4));
            pokeName = gen2GameReader_.getPokemonName(pokeIndex);
            shiny = gen2_isPokemonShiny(context_->poke_g2);
            snprintf(pokeStatsString_, sizeof(pokeStatsString_), "ATK:            %u\nDEF:            %u\nSPEC. ATK:  %u\nSPEC. DEF:  %u\nSPEED:        %u", atk, def, specAtk, specDef, speed);
            break;
        default:
            deps_.tpakManager.setRAMEnabled(false);
            return;
    }
    deps_.tpakManager.setRAMEnabled(false);

    loadPokemonSprite(pokeIndex, shiny);

    shinyText = (shiny) ? "Shiny " : "";
    snprintf(nameBuffer_, sizeof(nameBuffer_) - 1, "%s%s", shinyText, pokeName);
    snprintf(levelAndNumberBuffer_, sizeof(levelAndNumberBuffer_), "L %hu No. %hu\nHP:  %u/%u", level, pokeNumber, hp, hp);
    snprintf(otInfoString_, sizeof(otInfoString_), "TID: %u\nOT: %s", trainerID, context_->trainerName);
    snprintf(movesString, sizeof(movesString), "%s\n%s\n%s\n%s", move1Str, move2Str, move3Str, move4Str);

    if(context_->showReceivedPokemonDialog)
    {
        if(context_->isEgg)
        {
            setDialogDataText(diag_, "%s received a %s%s EGG!Take good care of it!", trainerName, shinyText, pokeName);
        }
        else
        {
            setDialogDataText(diag_, "%s received a %s%s!\nTake good care of it!", trainerName, shinyText, pokeName);
        }
        showDialog(&diag_);
    }
}

void StatsScene::destroy()
{
    SceneWithDialogWidget::destroy();
    
    sprite_free(menu9SliceSprite_);
    menu9SliceSprite_ = nullptr;

    if(pokeSpriteSurface_.buffer)
    {
        surface_free(&pokeSpriteSurface_);
    }
}

void StatsScene::render(RDPQGraphics& gfx, const Rectangle& sceneBounds)
{
    SceneWithDialogWidget::render(gfx, sceneBounds);

    gfx.drawSprite(backgroundBounds, menu9SliceSprite_, backgroundRenderSettings_);
    gfx.drawSurface(spriteBounds_, &pokeSpriteSurface_, {0});

    gfx.drawText(nameBounds, nameBuffer_, textSettings_);
    gfx.drawText(levelAndNumberBounds, levelAndNumberBuffer_, smallTextSettings_);
    gfx.drawText(otInfoBounds, otInfoString_, smallTextSettings_);
    gfx.drawText(statsBounds, pokeStatsString_, smallTextSettings_);
    gfx.drawText(movesBounds, movesString, smallTextSettings_);
}

void StatsScene::onDialogFinished()
{
    deps_.sceneManager.goBackToPreviousScene();
}

void StatsScene::setupDialog(DialogWidgetStyle& style)
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

void StatsScene::loadPokemonSprite(uint8_t pokeIndex, bool shiny)
{
    SpriteRenderer renderer;
    uint8_t* spriteBuffer;
    uint8_t* outputBuffer;
    uint16_t colorPalette[4];
    uint8_t spriteWidthInTiles;
    uint8_t spriteHeightInTiles;
    uint8_t spriteWidthInPixels;
    uint8_t spriteHeightInPixels;
    const OutputFormat outputFormat = OutputFormat::RGBA16;
    const uint8_t numBytesPerColor = getNumBytesPerColorFor(outputFormat);
    
    switch(deps_.generation)
    {
        case 1:
        {
            Gen1PokeStats pokeStats;
            gen1GameReader_.readPokemonStatsForIndex(pokeIndex, pokeStats);
            gen1GameReader_.readColorPalette(gen1GameReader_.getColorPaletteIndexByPokemonNumber(pokeStats.pokedex_number), colorPalette);
            spriteBuffer = gen1GameReader_.decodeSprite(pokeStats.sprite_bank, pokeStats.pointer_to_frontsprite);
            spriteWidthInTiles = MAX_SPRITE_TILES_WIDTH;
            spriteHeightInTiles = MAX_SPRITE_TILES_HEIGHT;
            break;
        }
        case 2:
        {
            Gen2PokeStats pokeStats;
            uint8_t bankIndex;
            uint16_t bankPointer;
            gen2GameReader_.readPokemonStatsForIndex(pokeIndex, pokeStats);
            gen2GameReader_.readFrontSpritePointer(pokeIndex, bankIndex, bankPointer);
            gen2GameReader_.readSpriteDimensions(pokeStats, spriteWidthInTiles, spriteHeightInTiles);
            gen2GameReader_.readColorPaletteForPokemon(pokeIndex, shiny, colorPalette);
            spriteBuffer = gen2GameReader_.decodeSprite(bankIndex, bankPointer);
            break;
        }
        default:
            return;
    }

    renderer.draw(spriteBuffer, outputFormat, colorPalette, spriteWidthInTiles, spriteHeightInTiles);
    outputBuffer = renderer.removeWhiteBackground(spriteWidthInTiles, spriteHeightInTiles);
    
    spriteWidthInPixels = spriteWidthInTiles * 8;
    spriteHeightInPixels = spriteHeightInTiles * 8;
    
    // due to alignment constraints, we should use the surface_alloc function to ensure that it is done correctly
    // and respect its resulting stride field
    pokeSpriteSurface_ = surface_alloc(FMT_RGBA16, spriteWidthInPixels, spriteHeightInPixels);
    spriteBounds_.width = spriteWidthInPixels;
    spriteBounds_.height = spriteHeightInPixels;

    // now copy the sprite to the surface
    const uint32_t actualStride = spriteWidthInPixels * numBytesPerColor;
    for(uint8_t i=0; i < spriteHeightInPixels; ++i)
    {
        uint8_t* src = outputBuffer + (i * actualStride);
        uint8_t* dst = ((uint8_t*)(pokeSpriteSurface_.buffer)) + (i * pokeSpriteSurface_.stride);
        // copy the source buffer to the surface line by line, but advancing with the surface stride on every new row
        memcpy(dst, src, actualStride);
    }
}

void deleteStatsSceneContext(void* context)
{
    auto sceneContext = static_cast<StatsSceneContext*>(context);
    delete sceneContext;
}
