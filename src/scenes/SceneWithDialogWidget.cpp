#include "scenes/SceneWithDialogWidget.h"
#include "scenes/SceneManager.h"
#include "core/FontManager.h"

SceneWithDialogWidget::SceneWithDialogWidget(SceneDependencies& deps)
    : AbstractUIScene(deps)
    , dialogWidget_(deps.animationManager)
    , dialogFocusChainSegment_({
        .current = &dialogWidget_
    })
    , mainFontId_(1)
    , fontStyleWhiteId_(0)
    , fontStyleYellowId_(1)
{
}

SceneWithDialogWidget::~SceneWithDialogWidget()
{
}

void SceneWithDialogWidget::init()
{
    DialogWidgetStyle style = {
        .dialogOptions = {
            .bounds = Rectangle{190, 179, 120, 0},
            .style = {
                .size = {140, 16},
                .titleNotFocused = {
                    .fontId = mainFontId_,
                    .fontStyleId = fontStyleWhiteId_
                },
                .titleFocused = {
                    .fontId = mainFontId_,
                    .fontStyleId = fontStyleYellowId_
                },
                .leftMargin = 5,
                .topMargin = 1
            }
        },
        .textSettings = {
            .fontId = mainFontId_,
            .fontStyleId = fontStyleWhiteId_
        },
        .margin = {
            .left = 10,
            .right = 10,
            .top = 10,
            .bottom = 10
        }
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

void SceneWithDialogWidget::advanceDialog()
{
    dialogWidget_.advanceDialog();
}

void SceneWithDialogWidget::showDialog(DialogData* diagData)
{
    dialogWidget_.setVisible(true);
    dialogWidget_.setData(diagData);
}

void SceneWithDialogWidget::setupFonts()
{
    mainFontId_ = deps_.fontManager.getFont("rom://Arial.font64");

    const rdpq_fontstyle_t mainFontWhite = {
        .color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF),
        .outline_color = RGBA32(0, 0, 0, 0xFF)
    };
    const rdpq_fontstyle_t mainFontYellow = {
        .color = RGBA32(0xFF, 0xFF, 0x00, 0xFF),
        .outline_color = RGBA32(0, 0, 0, 0xFF)
    };

    deps_.fontManager.registerFontStyle(mainFontId_, fontStyleWhiteId_, mainFontWhite);
    deps_.fontManager.registerFontStyle(mainFontId_, fontStyleYellowId_, mainFontYellow);
}

void SceneWithDialogWidget::setupDialog(DialogWidgetStyle& style)
{
    dialogWidget_.setStyle(style);
    dialogWidget_.setBounds(Rectangle{10, 180, 300, 50});
}