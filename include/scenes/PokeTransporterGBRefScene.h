#ifndef _POKETRANSPORTERGBREFSCENE_H
#define _POKETRANSPORTERGBREFSCENE_H

#include "scenes/SceneWithDialogWidget.h"

class PokeTransporterGBRefScene : public SceneWithDialogWidget
{
public:
    PokeTransporterGBRefScene(SceneDependencies& deps, void* context);
    virtual ~PokeTransporterGBRefScene();

    void init() override;
    void destroy() override;

    void render(RDPQGraphics& gfx, const Rectangle& sceneBounds) override;

    void onDialogDone();
protected:
    void setupDialog(DialogWidgetStyle& style) override;
private:
    sprite_t* menu9SliceSprite_;
    sprite_t* pokeTransporterGBLogoSprite_;
    sprite_t* qrCodeSprite_;
    sprite_t* fennelPictureSprite_;
    uint8_t fontArialSmallId_;
    uint8_t fontArialSmallWhiteId_;
};

#endif