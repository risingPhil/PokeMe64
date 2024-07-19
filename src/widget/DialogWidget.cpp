#include "widget/DialogWidget.h"
#include "core/RDPQGraphics.h"

#include <cstdarg>

DialogWidget::DialogWidget(AnimationManager& animationManager)
    : animationManager_(animationManager)
    , bounds_({0})
    , style_({0})
    , data_(nullptr)
    , onDialogFinishedCb_(nullptr)
    , onDialogFinishedCbContext_(nullptr)
    , focused_(false)
    , visible_(true)
    , btnAPressedOnPrevCheck_(false)
{
}

DialogWidget::~DialogWidget()
{
}

const DialogWidgetStyle& DialogWidget::getStyle() const
{
    return style_;
}

void DialogWidget::setStyle(const DialogWidgetStyle& style)
{
    style_ = style;
}

void DialogWidget::setData(DialogData* data)
{
    data_ = data;
}

void DialogWidget::appendDialogData(DialogData* data)
{
    if(!data_)
    {
        setData(data);
        return;
    }

    DialogData* entry = data_;
    while(entry->next)
    {
        entry = entry->next;
    }
    entry->next = data;
}

bool DialogWidget::isFocused() const
{
    return focused_;
}

void DialogWidget::setFocused(bool focused)
{
    focused_ = focused;
}

bool DialogWidget::isVisible() const
{
    return visible_;
}

void DialogWidget::setVisible(bool visible)
{
    visible_ = visible;
}

Rectangle DialogWidget::getBounds() const
{
    return bounds_;
}

void DialogWidget::setBounds(const Rectangle& bounds)
{
    bounds_ = bounds;
}

Dimensions DialogWidget::getSize() const
{
    return Dimensions{.width = bounds_.width, .height = bounds_.height};
}

void DialogWidget::setOnDialogFinishedCallback(void (*onDialogFinishedCb)(void*), void* context)
{
    onDialogFinishedCb_ = onDialogFinishedCb;
    onDialogFinishedCbContext_ = context;
}

void DialogWidget::advanceDialog()
{
    if(!data_ || !data_->next)
    {
        if(onDialogFinishedCb_)
        {
            onDialogFinishedCb_(onDialogFinishedCbContext_);
        }
        return;
    }
    const DialogData* oldEntry = data_;
    data_ = data_->next;

    if(oldEntry->shouldReleaseWhenDone)
    {
        delete oldEntry;
        oldEntry = nullptr;
    }
}

bool DialogWidget::handleUserInput(const joypad_inputs_t& userInput)
{
    // make sure the user needs to release the button before handling the A button again
    // if we don't do that, a different component might react to the same a button press
    if(btnAPressedOnPrevCheck_)
    {
        if(!userInput.btn.a)
        {
            advanceDialog();
            btnAPressedOnPrevCheck_ = false;
        }
    }
    else if(isAdvanceAllowed() && userInput.btn.a)
    {
        btnAPressedOnPrevCheck_ = true;
    }

    return false;
}

void DialogWidget::render(RDPQGraphics& gfx, const Rectangle& parentBounds)
{
    if(!visible_)
    {
        return;
    }

    const Rectangle myBounds = addOffset(bounds_, parentBounds);
    // render the background first, if any.
    if(style_.backgroundSprite)
    {
        gfx.drawSprite(myBounds, style_.backgroundSprite, style_.backgroundSpriteSettings);
    }

    if(!data_)
    {
        return;
    }

    if(data_->characterSprite && data_->characterSpriteVisible)
    {
        const Rectangle absoluteCharBounds = addOffset(data_->characterSpriteBounds, myBounds);
        gfx.drawSprite(absoluteCharBounds, data_->characterSprite, data_->characterSpriteSettings);
    }

    if(data_->buttonSprite && data_->buttonSpriteVisible)
    {
        const Rectangle absoluteButtonSpriteBounds = addOffset(data_->buttonSpriteBounds, myBounds);
        gfx.drawSprite(absoluteButtonSpriteBounds, data_->buttonSprite, data_->buttonSpriteSettings);
    }

    if(data_->text[0] != '\0')
    {
        const Rectangle textBounds = {
            .x = myBounds.x + style_.marginLeft,
            .y = myBounds.y + style_.marginTop,
            .width = myBounds.width - style_.marginLeft - style_.marginRight,
            .height = myBounds.height - style_.marginTop - style_.marginBottom
        };

        gfx.drawText(textBounds, data_->text, style_.textSettings);
    }
}

bool DialogWidget::isAdvanceAllowed() const
{
    return (!data_ || !data_->userAdvanceBlocked);
}

void setDialogDataText(DialogData& data, const char* format, ...)
{
    va_list argList;
    va_start(argList, format);

    vsnprintf(data.text, DIALOG_TEXT_SIZE, format, argList);

    va_end(argList);
}
