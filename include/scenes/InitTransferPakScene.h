#ifndef _INITTRANSFERPAKSCENE_H
#define _INITTRANSFERPAKSCENE_H

#include "scenes/SceneWithDialogWidget.h"
#include "widget/TransferPakDetectionWidget.h"

#define PLAYER_NAME_SIZE 15

class TransferPakManager;

/**
 * @brief In this scene implementation, we do the detection of the N64 transfer pak and whether a supported Pokémon game was
 * detected.
 */
class InitTransferPakScene : public SceneWithDialogWidget
{
public:
    InitTransferPakScene(SceneDependencies& deps, void* context);
    virtual ~InitTransferPakScene();

    void init() override;
    void destroy() override;

    void render(RDPQGraphics& gfx, const Rectangle& sceneBounds) override;

    void onDialogDone();
    void onTransferPakWidgetStateChanged(TransferPakWidgetState newState);
protected:
private:
    void setupTPakDetectWidget();
    void setupDialog(DialogWidgetStyle& style) override;

    void loadGameMetadata();
    const char* getGameTypeString();

    sprite_t* menu9SliceSprite_;
    TransferPakDetectionWidget tpakDetectWidget_;
    WidgetFocusChainSegment tpakDetectWidgetSegment_;
    DialogData diagData_;
    TextRenderSettings pokeMe64TextSettings_;
    char playerName_[PLAYER_NAME_SIZE];
    const char* gameTypeString_;
};

#endif