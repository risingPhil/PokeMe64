#include "scenes/PokeTransporterGBRefScene.h"
#include "scenes/SceneManager.h"
#include "core/RDPQGraphics.h"
#include "core/FontManager.h"

static const Rectangle logoRectangle = {96, 10, 128, 39};
static const Rectangle fennelPictureRectangle = {20, 60, 62, 110};
static const Rectangle qrCodeRectangle = {160, 65, 66, 66};
static const Rectangle urlRectangle = {100, 140, 210, 32};

static void dialogFinishedCallback(void* context)
{
    PokeTransporterGBRefScene* scene = (PokeTransporterGBRefScene*)context;
    scene->onDialogDone();
}

PokeTransporterGBRefScene::PokeTransporterGBRefScene(SceneDependencies& deps, void* context)
    : SceneWithDialogWidget(deps)
    , menu9SliceSprite_(nullptr)
    , pokeTransporterGBLogoSprite_(nullptr)
    , qrCodeSprite_(nullptr)
    , fennelPictureSprite_(nullptr)
    , fontArialSmallId_(0)
    , fontArialSmallWhiteId_(0)
{
}

PokeTransporterGBRefScene::~PokeTransporterGBRefScene()
{
}

void PokeTransporterGBRefScene::init()
{
    menu9SliceSprite_ = sprite_load("rom://menu-bg-9slice.sprite");
    pokeTransporterGBLogoSprite_ = sprite_load("rom://logo-poketransporter-gb.sprite");
    qrCodeSprite_ = sprite_load("rom://qrcode-poketransporter-gb.sprite");
    fennelPictureSprite_ = sprite_load("rom://fennel-picture.sprite");
    fontArialSmallId_ = deps_.fontManager.getFont("rom://Arial-small.font64");
    
    const rdpq_fontstyle_t arialWhite = {
        .color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF),
        .outline_color = RGBA32(0, 0, 0, 0xFF)
    };
    deps_.fontManager.registerFontStyle(fontArialSmallId_, fontArialSmallWhiteId_, arialWhite);

    SceneWithDialogWidget::init();

    auto diag2 = new DialogData{
        .shouldDeleteWhenDone = true
    };
    auto diag1 = new DialogData {
        .next = diag2,
        .shouldDeleteWhenDone = true
    };

    setDialogDataText(*diag1, "So you want to transfer your pokémon to\nGen 3? You should go see professor Fennel!");
    setDialogDataText(*diag2, "You can find the address above or you can scan the QR code with your phone!");

    showDialog(diag1);
}

void PokeTransporterGBRefScene::destroy()
{
    sprite_free(fennelPictureSprite_);
    fennelPictureSprite_ = nullptr;

    sprite_free(qrCodeSprite_);
    qrCodeSprite_ = nullptr;

    sprite_free(pokeTransporterGBLogoSprite_);
    pokeTransporterGBLogoSprite_ = nullptr;

    sprite_free(menu9SliceSprite_);
    menu9SliceSprite_ = nullptr;
}

void PokeTransporterGBRefScene::render(RDPQGraphics& gfx, const Rectangle& sceneBounds)
{
    SpriteRenderSettings renderSettings = {
        .renderMode = SpriteRenderMode::NORMAL
    };
    TextRenderSettings textSettings = {
        .fontId = fontArialSmallId_,
        .fontStyleId = fontArialSmallWhiteId_
    };

    gfx.drawSprite(logoRectangle, pokeTransporterGBLogoSprite_, renderSettings);
    gfx.drawSprite(fennelPictureRectangle, fennelPictureSprite_, renderSettings);
    gfx.drawSprite(qrCodeRectangle, qrCodeSprite_, renderSettings);
    gfx.drawText(urlRectangle, "https://github.com/GearsProgress\n/Poke_Transporter_GB", textSettings);
    
    SceneWithDialogWidget::render(gfx, sceneBounds);
}

void PokeTransporterGBRefScene::onDialogDone()
{
    deps_.sceneManager.goBackToPreviousScene();
}

void PokeTransporterGBRefScene::setupDialog(DialogWidgetStyle& style)
{
    style.background.sprite = menu9SliceSprite_;
    style.background.spriteSettings = {
        .renderMode = SpriteRenderMode::NINESLICE,
        .srcRect = { 6, 6, 6, 6 }
    };

    SceneWithDialogWidget::setupDialog(style);

    dialogWidget_.setOnDialogFinishedCallback(dialogFinishedCallback, this);
}