#include "widget/ImageWidget.h"

ImageWidget::ImageWidget()
    : style_({0})
    , bounds_({0})
    , onConfirmAction_(nullptr)
    , onConfirmActionContext_(nullptr)
    , focused_(false)
    , visible_(true)
    , aButtonPressed_(false)
{
}

ImageWidget::~ImageWidget()
{
}

const ImageWidgetStyle& ImageWidget::getStyle() const
{
    return style_;
}

void ImageWidget::setStyle(const ImageWidgetStyle& style)
{
    style_ = style;
}

bool ImageWidget::isFocused() const
{
    return focused_;
}

void ImageWidget::setFocused(bool focused)
{
    focused_ = focused;
}

bool ImageWidget::isVisible() const
{
    return visible_;
}

void ImageWidget::setVisible(bool visible)
{
    visible_ = visible;
}

Rectangle ImageWidget::getBounds() const
{
    return bounds_;
}

void ImageWidget::setBounds(const Rectangle& bounds)
{
    bounds_ = bounds;
}

Dimensions ImageWidget::getSize() const
{
    return style_.size;
}

void ImageWidget::setConfirmAction(void (*onConfirmAction)(void*), void* context)
{
    onConfirmAction_ = onConfirmAction;
    onConfirmActionContext_ = context;
}

bool ImageWidget::handleUserInput(const joypad_inputs_t& userInput)
{
    if(!focused_)
    {
        return false;
    }

    const bool shouldExecuteConfirmAction = aButtonPressed_ && !userInput.btn.a;
    aButtonPressed_ = static_cast<bool>(userInput.btn.a);
    if(shouldExecuteConfirmAction)
    {
        if(onConfirmAction_)
        {
            onConfirmAction_(onConfirmActionContext_);
        }
    }
    return (shouldExecuteConfirmAction && onConfirmAction_);
}

void ImageWidget::render(RDPQGraphics& gfx, const Rectangle& parentBounds)
{
    if(!visible_)
    {
        return;
    }
    const Rectangle absoluteBounds = addOffset(bounds_, parentBounds);
    if(style_.background.sprite && (focused_ || style_.background.visibleWhenNotFocused))
    {
        gfx.drawSprite(absoluteBounds, style_.background.sprite, style_.background.spriteSettings);
    }

    if(style_.image.sprite)
    {
        const Rectangle iconSpriteBounds = addOffset(style_.image.spriteBounds, absoluteBounds);
        gfx.drawSprite(iconSpriteBounds, style_.image.sprite, style_.image.spriteSettings);
    }
}
