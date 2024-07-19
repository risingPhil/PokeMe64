#include "scenes/TestScene.h"
#include "core/RDPQGraphics.h"

#include <cstdio>
#include <n64sys.h>

static const char* tvtypeToString(tv_type_t type)
{
    switch(type)
    {
        case TV_PAL:
            return "PAL";
        case TV_NTSC:
            return "NTSC";
        case TV_MPAL:
            return "MPAL";
        default:
            return "INVALID";
    }
}

TestScene::TestScene(SceneDependencies& deps, void*)
    : AbstractUIScene(deps)
    , arialFont_(nullptr)
    , arialFontId_(1)
    , fontStyleWhite_(0)
    , pokeballSprite_(nullptr)
    , oakSprite_(nullptr)
    , menu9SliceSprite_(nullptr)
    , rectBounds_({0, 0, 320, 240})
    , textRect_({70, 70, 180, 60})
    , spriteBounds_({320 - 128, 0, 128, 128})
    , oakBounds_({0})
    , oakSrcBounds_({0})
    , menuBounds_({ 100, 20, 100, 100})
    , textRenderSettings_({})
    , menuRenderSettings_({
        .renderMode = SpriteRenderMode::NINESLICE,
        .srcRect = { 6, 6, 6, 6 }
    })
{
}

TestScene::~TestScene()
{
}

void TestScene::init()
{
    arialFont_ = rdpq_font_load("rom://Arial.font64");
    rdpq_fontstyle_t arialWhiteFontStyle = {
        .color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF)
    };
    rdpq_font_style(arialFont_, fontStyleWhite_, &arialWhiteFontStyle);

    // TODO: this is a problem: there's no way to unregister a font.
    // Therefore if we'd load the same font > 1 times, we'll get a crash (due to assert)
    // We'll need to create a FontManager class to handle this
    rdpq_text_register_font(arialFontId_, arialFont_);
    textRenderSettings_.fontId = arialFontId_;

    pokeballSprite_ = sprite_load("rom:/pokeball.sprite");
    oakSprite_ = sprite_load("rom://oak.sprite");
    menu9SliceSprite_ = sprite_load("rom://menu-bg-9slice.sprite");

    oakBounds_ ={10, 80, oakSprite_->width, oakSprite_->height};
    oakSrcBounds_ = {oakSprite_->width / 3, oakSprite_->height / 3, oakSprite_->width * 2 / 3, oakSprite_->height * 2 / 3};

    printf("Hello Phil! Your tv type is: %s\n", tvtypeToString(get_tv_type()));
}

void TestScene::destroy()
{
    sprite_free(menu9SliceSprite_);
    menu9SliceSprite_ = nullptr;

    sprite_free(oakSprite_);
    oakSprite_ = nullptr;

    sprite_free(pokeballSprite_);
    pokeballSprite_ = nullptr;

    rdpq_font_free(arialFont_);
}

void TestScene::render(RDPQGraphics& gfx, const Rectangle& /*sceneBounds*/)
{
//    gfx.fillRectangle(rectBounds_, RGBA32(200, 0, 0, 0));
//    gfx.drawText(textRect_, "Hello Phil!", textRenderSettings_);
//    gfx.drawSprite(spriteBounds_, pokeballSprite_, SpriteRenderSettings());
//    gfx.drawSprite(oakBounds_, oakSprite_, {.srcRect = oakSrcBounds_});
    gfx.drawSprite(menuBounds_, menu9SliceSprite_, menuRenderSettings_);
}