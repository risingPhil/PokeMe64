#ifndef _STATSSCENE_H
#define _STATSSCENE_H

#include "scenes/SceneWithDialogWidget.h"
#include "gen1/Gen1Common.h"
#include "gen2/Gen2Common.h"
#include "core/Sprite.h"
#include "transferpak/TransferPakRomReader.h"
#include "transferpak/TransferPakSaveManager.h"
#include "gen1/Gen1GameReader.h"
#include "gen2/Gen2GameReader.h"

typedef struct StatsSceneContext
{
    union {
        Gen1TrainerPokemon poke_g1;
        Gen2TrainerPokemon poke_g2;
    };
    char trainerName[15];
    bool showReceivedPokemonDialog;
    bool isEgg;
} StatsSceneContext;

/**
 * Shows the stats for a pokemon
 */
class StatsScene : public SceneWithDialogWidget
{
public:
    StatsScene(SceneDependencies& deps, void* context);
    virtual ~StatsScene();

    void init() override;
    void destroy() override;

    void render(RDPQGraphics& gfx, const Rectangle& sceneBounds) override;

    void onDialogFinished();
protected:
    void setupDialog(DialogWidgetStyle& style) override;
private:
    void loadPokemonSprite(uint8_t pokeIndex, bool shiny);

    DialogData diag_;
    TransferPakRomReader romReader_;
    TransferPakSaveManager saveManager_;
    Gen1GameReader gen1GameReader_;
    Gen2GameReader gen2GameReader_;
    sprite_t* menu9SliceSprite_;
    SpriteRenderSettings backgroundRenderSettings_;
    uint8_t fontArialSmallId_;
    uint8_t fontArialSmallWhiteId_;
    TextRenderSettings textSettings_;
    TextRenderSettings smallTextSettings_;
    TextRenderSettings statsSettings_;
    Rectangle spriteBounds_;
    char nameBuffer_[25];
    char levelAndNumberBuffer_[40];
    char pokeStatsString_[150];
    char otInfoString_[40];
    char movesString[256];
    StatsSceneContext* context_;
    surface_t pokeSpriteSurface_;

};

void deleteStatsSceneContext(void* context);

#endif