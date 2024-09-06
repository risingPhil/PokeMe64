#include "scenes/SceneWithProgressBar.h"

static const Rectangle progressBarBounds = {50, 75, 220, 30};

SceneWithProgressBar::SceneWithProgressBar(SceneDependencies& deps)
    : SceneWithDialogWidget(deps)
    , progressWidget_()
{
}

SceneWithProgressBar::~SceneWithProgressBar()
{
}

void SceneWithProgressBar::init()
{
    ProgressBarWidgetStyle progressStyle = {0};
    
    SceneWithDialogWidget::init();

    setupProgressBar(progressStyle);
    progressWidget_.setStyle(progressStyle);
}

void SceneWithProgressBar::render(RDPQGraphics& gfx, const Rectangle& sceneBounds)
{
    progressWidget_.render(gfx, sceneBounds);
    SceneWithDialogWidget::render(gfx, sceneBounds);
}

void SceneWithProgressBar::setProgress(double progress)
{
    progressWidget_.setProgress(progress);
}

void SceneWithProgressBar::setupProgressBar(ProgressBarWidgetStyle& style)
{
    style.bar = {
        .margin = { 3, 3, 3, 3 },
        .color = RGBA32(0x0, 0x61, 0xFF, 0xFF)
    };
    style.textSettings = {
        .fontId = arialId_,
        .fontStyleId = fontStyleWhiteId_,
        .halign = ALIGN_CENTER,
        .valign = VALIGN_CENTER
    };

    progressWidget_.setBounds(progressBarBounds);
}