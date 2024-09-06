#include "widget/ProgressBarWidget.h"

ProgressBarWidget::ProgressBarWidget()
    : style_({0})
    , visible_(true)
    , bounds_({0})
    , progress_()
    , textBuffer_()
{
    setProgress(0);
}

ProgressBarWidget::~ProgressBarWidget()
{
}

void ProgressBarWidget::setProgress(double progress)
{
    progress_ = progress;

    snprintf(textBuffer_, sizeof(textBuffer_), "%hu%%", static_cast<uint8_t>(progress * 100.0));
}

void ProgressBarWidget::setStyle(const ProgressBarWidgetStyle& style)
{
    style_ = style;
}

bool ProgressBarWidget::isFocused() const
{
    return false;
}

void ProgressBarWidget::setFocused(bool)
{
    //dummy
}

bool ProgressBarWidget::isVisible() const
{
    return visible_;
}

void ProgressBarWidget::setVisible(bool visible)
{
    visible_ = visible;
}

Rectangle ProgressBarWidget::getBounds() const
{
    return bounds_;
}

void ProgressBarWidget::setBounds(const Rectangle& bounds)
{
    bounds_ = bounds;
}

Dimensions ProgressBarWidget::getSize() const
{
    return Dimensions{bounds_.width, bounds_.height};
}

bool ProgressBarWidget::handleUserInput(const joypad_inputs_t&)
{
    return false;
}

void ProgressBarWidget::render(RDPQGraphics& gfx, const Rectangle& parentBounds)
{
    if(!visible_)
    {
        return;
    }

    const Rectangle myAbsoluteBounds = addOffset(bounds_, parentBounds);
    const Rectangle foregroundRectangle = {
        .x = myAbsoluteBounds.x + style_.bar.margin.left,
        .y = myAbsoluteBounds.y + style_.bar.margin.top,
        .width = bounds_.width - style_.bar.margin.left - style_.bar.margin.right,
        .height = bounds_.height - style_.bar.margin.top - style_.bar.margin.bottom
    };

    // draw background
    if(style_.background.sprite)
    {
        gfx.drawSprite(myAbsoluteBounds, style_.background.sprite, style_.background.renderSettings);
    }

    // draw foreground
    if(progress_)
    {
        const Rectangle progressRectangle = {
            .x = foregroundRectangle.x,
            .y = foregroundRectangle.y,
            .width = static_cast<int>(foregroundRectangle.width * progress_),
            .height = foregroundRectangle.height
        };

        if(progressRectangle.width && progressRectangle.height)
        {
            if(style_.bar.sprite)
            {
                gfx.drawSprite(progressRectangle, style_.bar.sprite, style_.bar.spriteSettings);
            }
            else
            {
                gfx.fillRectangle(progressRectangle, style_.bar.color);
            }
        }
    }

    if(style_.textSettings.fontId)
    {
        gfx.drawText(foregroundRectangle, textBuffer_, style_.textSettings);
    }
}