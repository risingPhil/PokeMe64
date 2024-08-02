#include "widget/TextWidget.h"

TextWidget::TextWidget()
    : style_({0})
    , bounds_({0})
    , text_(nullptr)
    , onConfirmAction_(nullptr)
    , onConfirmActionContext_(nullptr)
    , focused_(false)
    , visible_(true)
    , aButtonPressed_(false)
{
}

TextWidget::~TextWidget()
{
}

const TextWidgetStyle& TextWidget::getStyle() const
{
    return style_;
}

void TextWidget::setStyle(const TextWidgetStyle& style)
{
    style_ = style;
}

void TextWidget::setData(const char* text)
{
    text_ = text;
}

const char* TextWidget::getData() const
{
    return text_;
}

bool TextWidget::isFocused() const
{
    return focused_;
}

void TextWidget::setFocused(bool focused)
{
    focused_ = focused;
}

bool TextWidget::isVisible() const
{
    return visible_;
}

void TextWidget::setVisible(bool visible)
{
    visible_ = visible;
}

Rectangle TextWidget::getBounds() const
{
    return bounds_;
}

void TextWidget::setBounds(const Rectangle& bounds)
{
    bounds_ = bounds;
}

Dimensions TextWidget::getSize() const
{
    return Dimensions{.width = bounds_.width, .height = bounds_.height};
}

void TextWidget::setConfirmAction(void (*onConfirmAction)(void*), void* context)
{
    onConfirmAction_ = onConfirmAction;
    onConfirmActionContext_ = context;
}

bool TextWidget::handleUserInput(const joypad_inputs_t& userInput)
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

void TextWidget::render(RDPQGraphics& gfx, const Rectangle& parentBounds)
{
    if(!visible_)
    {
        return;
    }
    TextRenderSettings& renderSettings = (focused_) ? style_.renderSettingsFocused : style_.renderSettingsNotFocused;

    const Rectangle absoluteBounds = addOffset(bounds_, parentBounds);
    gfx.drawText(absoluteBounds, text_, renderSettings);
}