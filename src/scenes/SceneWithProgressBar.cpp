#include "scenes/SceneWithProgressBar.h"
#include "scenes/SceneManager.h"
#include "core/DragonUtils.h"
#include "core/FontManager.h"

static void dialogFinishedCallback(void* context)
{
    SceneWithProgressBar* scene = (SceneWithProgressBar*)context;
    scene->onDialogDone();
}

SceneWithProgressBar::SceneWithProgressBar(SceneDependencies& deps)
    : SceneWithDialogWidget(deps)
    , progressWidget_()
    , dialogWidgetSprite_(nullptr)
    , progressBackgroundSprite_(nullptr)
    , progress_(0)
    , arialSmallId_(2)
{
}

SceneWithProgressBar::~SceneWithProgressBar()
{
}

void SceneWithProgressBar::init()
{
    dialogWidgetSprite_ = sprite_load("rom://menu-bg-9slice.sprite");
    progressBackgroundSprite_ = sprite_load("rom://bg-nineslice-transparant-border.sprite");

    SceneWithDialogWidget::init();

    const ProgressBarWidgetStyle progressStyle = {
        .background = {
            .sprite = progressBackgroundSprite_,
            .renderSettings = {
                .renderMode = SpriteRenderMode::NINESLICE,
                .srcRect = { 6, 6, 6, 6 }
            }
        },
        .bar = {
            .margin = { 3, 3, 3, 3 },
            .color = RGBA32(0x0, 0x61, 0xFF, 0xFF)
        },
        .textSettings = {
            .fontId = arialId_,
            .fontStyleId = fontStyleWhiteId_,
            .halign = ALIGN_CENTER,
            .valign = VALIGN_CENTER
        }
    };

    progressWidget_.setStyle(progressStyle);
    progressWidget_.setBounds({50, 75, 220, 30});

    arialSmallId_ = deps_.fontManager.getFont("rom://Arial-small.font64");
    const rdpq_fontstyle_t arialWhite = {
        .color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF),
        .outline_color = RGBA32(0, 0, 0, 0xFF)
    };
    deps_.fontManager.registerFontStyle(arialSmallId_, fontStyleWhiteId_, arialWhite);
}

void SceneWithProgressBar::destroy()
{
    sprite_free(dialogWidgetSprite_);
    dialogWidgetSprite_ = nullptr;
    sprite_free(progressBackgroundSprite_);
    progressBackgroundSprite_ = nullptr;
}

bool SceneWithProgressBar::handleUserInput(joypad_port_t port, const joypad_inputs_t& inputs)
{
    const UINavigationDirection navDirection = determineUINavigationDirection(inputs, NavigationInputSourceType::BOTH);
    bool handled;

    switch(navDirection)
    {
        case UINavigationDirection::LEFT:
            progress_ = (progress_ > 0) ? progress_ - 1 : 0;
            handled = true;
            break;
        case UINavigationDirection::RIGHT:
            progress_ = (progress_ < 100) ? progress_ + 1 : 100;
            handled = true;
            break;
        default:
            handled = false;
            break;
    }

    if(handled)
    {
        progressWidget_.setProgress(progress_);
        return handled;
    }
    else
    {
        return SceneWithDialogWidget::handleUserInput(port, inputs);
    }
}

void SceneWithProgressBar::render(RDPQGraphics& gfx, const Rectangle& sceneBounds)
{
    progressWidget_.render(gfx, sceneBounds);
    SceneWithDialogWidget::render(gfx, sceneBounds);
}

void SceneWithProgressBar::onDialogDone()
{
    deps_.sceneManager.goBackToPreviousScene();
}

void SceneWithProgressBar::setupFonts()
{
    SceneWithDialogWidget::setupFonts();


}

void SceneWithProgressBar::setupDialog(DialogWidgetStyle& style)
{
    style.background.sprite = dialogWidgetSprite_;
    style.background.spriteSettings = {
        .renderMode = SpriteRenderMode::NINESLICE,
        .srcRect = { 6, 6, 6, 6 }
    };

    SceneWithDialogWidget::setupDialog(style);

    dialogWidget_.setOnDialogFinishedCallback(dialogFinishedCallback, this);
    dialogWidget_.setVisible(false);
}