#ifndef _TESTSCENE_H
#define _TESTSCENE_H

#include "scenes/AbstractUIScene.h"
#include "core/RDPQGraphics.h"
#include "core/Sprite.h"

class TestScene : public AbstractUIScene
{
public:
    TestScene(SceneDependencies& deps, void* sceneContext);
    virtual ~TestScene();

    void init() override;
    void destroy() override;

    void render(RDPQGraphics& gfx, const Rectangle& sceneBounds) override;
protected:
private:
    rdpq_font_t* arialFont_;
    uint8_t arialFontId_;
    uint8_t fontStyleWhite_;
    sprite_t* pokeballSprite_;
    sprite_t* oakSprite_;
    sprite_t* menu9SliceSprite_;
    const Rectangle rectBounds_;
    const Rectangle textRect_;
    const Rectangle spriteBounds_;
    Rectangle oakBounds_;
    Rectangle oakSrcBounds_;
    const Rectangle menuBounds_;
    TextRenderSettings textRenderSettings_;
    const SpriteRenderSettings menuRenderSettings_;
};

#endif