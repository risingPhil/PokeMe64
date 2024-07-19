#include "scenes/SceneWithDialogWidget.h"
#include "scenes/SceneManager.h"
#include "core/FontManager.h"

SceneWithDialogWidget::SceneWithDialogWidget(SceneDependencies& deps)
    : AbstractUIScene(deps)
    , dialogWidget_(deps.animationManager)
    , dialogFocusChainSegment_({
        .current = &dialogWidget_
    })
    , arialId_(1)
    , fontStyleWhiteId_(0)
{
}

SceneWithDialogWidget::~SceneWithDialogWidget()
{
}

void SceneWithDialogWidget::init()
{
    DialogWidgetStyle style = {
        .textSettings = {
            .fontId = arialId_,
            .fontStyleId = fontStyleWhiteId_
        },
        .marginLeft = 10,
        .marginRight = 10,
        .marginTop = 10,
        .marginBottom = 10
    };

    setupFonts();
    setupDialog(style);

    setFocusChain(&dialogFocusChainSegment_);
}

void SceneWithDialogWidget::destroy()
{
    dialogWidget_.setData(nullptr);
    dialogWidget_.setStyle({0});

    AbstractUIScene::destroy();
}

void SceneWithDialogWidget::render(RDPQGraphics& gfx, const Rectangle& sceneBounds)
{
    dialogWidget_.render(gfx, sceneBounds);
}

void SceneWithDialogWidget::setupFonts()
{
    arialId_ = deps_.fontManager.getFont("rom://Arial.font64");
    const rdpq_fontstyle_t arialWhite = {
        .color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF),
        .outline_color = RGBA32(0, 0, 0, 0xFF)
    };

    deps_.fontManager.registerFontStyle(arialId_, fontStyleWhiteId_, arialWhite);
}

void SceneWithDialogWidget::setupDialog(DialogWidgetStyle& style)
{
    dialogWidget_.setStyle(style);
    dialogWidget_.setBounds(Rectangle{10, 180, 300, 50});
}