#include "widget/DialogWidget.h"
#include "core/RDPQGraphics.h"

#include <cstdarg>

/**
 * @brief Releases either a single DialogData entry or its entire chain.
 */
static void releaseEntry(DialogData* data, bool releaseAllEntries)
{
    DialogData* cur = data;
    DialogData* next = nullptr;
    do
    {
        if(cur)
        {
            next = cur->next;
            if(cur->options.shouldDeleteWhenDone && cur->options.items)
            {
                delete[] cur->options.items;
                cur->options.items = nullptr;
            }
            if(cur->shouldDeleteWhenDone)
            {
                delete cur;
            }   
        }
        cur = next;

    } while(releaseAllEntries && cur);
}

DialogWidget::DialogWidget(AnimationManager& animationManager)
    : dialogOptionList_(animationManager)
    , dialogOptionListFiller_(dialogOptionList_)
    , animationManager_(animationManager)
    , bounds_({0})
    , style_({0})
    , data_(nullptr)
    , onDialogFinishedCb_(nullptr)
    , onDialogFinishedCbContext_(nullptr)
    , focused_(false)
    , visible_(true)
    , btnAPressedOnPrevCheck_(false)
{
    dialogOptionList_.setVisible(false);
}

DialogWidget::~DialogWidget()
{
    releaseEntry(data_, true);
    data_ = nullptr;
}

const DialogWidgetStyle& DialogWidget::getStyle() const
{
    return style_;
}

void DialogWidget::setStyle(const DialogWidgetStyle& style)
{
    style_ = style;
    
    VerticalListStyle& listStyle = dialogOptionList_.getStyle();
    listStyle.background.sprite = style.background.sprite;
    listStyle.background.spriteSettings = style.background.spriteSettings;
    listStyle.margin.left = style.margin.left;
    listStyle.margin.right = style.margin.right;
    listStyle.margin.top = style.margin.top;
    listStyle.margin.bottom = style.margin.bottom;
    listStyle.autogrow.enabled = true;
    listStyle.autogrow.shouldGrowUpWards = true;
    dialogOptionList_.setBounds(style.dialogOptions.bounds);
}

void DialogWidget::setData(DialogData* data)
{
    dialogOptionList_.clearWidgets();
    // the filler is the owner of the widgets. They need to be properly destroyed
    dialogOptionListFiller_.deleteWidgets();
    dialogOptionList_.setVisible(false);
    dialogOptionList_.setFocused(false);
    releaseEntry(data_, true);

    data_ = data;

    if(data_ && data->options.items)
    {
        dialogOptionListFiller_.addItems(data->options.items, data->options.number, style_.dialogOptions.style);
        dialogOptionList_.setVisible(true);
        dialogOptionList_.setFocused(focused_);
    }
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

    if(data_ && data_->options.number)
    {
        dialogOptionList_.setFocused(focused);
    }
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
    
    DialogData* oldEntry = data_;
    data_ = data_->next;
    
    releaseEntry(oldEntry, false);

    dialogOptionList_.clearWidgets();
    // the filler is the owner of the widgets. They need to be properly destroyed
    dialogOptionListFiller_.deleteWidgets();
    dialogOptionList_.setFocused(false);
    dialogOptionList_.setVisible(false);
    if(data_->options.items)
    {
        dialogOptionListFiller_.addItems(data_->options.items, data_->options.number, style_.dialogOptions.style);
        dialogOptionList_.setVisible(true);
        dialogOptionList_.setFocused(focused_);
    }

}

bool DialogWidget::handleUserInput(const joypad_inputs_t& userInput)
{
    // if dialog options are displayed and focused, the VerticalList widget needs to handle the key events
    if(dialogOptionList_.isFocused())
    {
        return dialogOptionList_.handleUserInput(userInput);
    }
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
    if(style_.background.sprite)
    {
        gfx.drawSprite(myBounds, style_.background.sprite, style_.background.spriteSettings);
    }

    if(!data_)
    {
        return;
    }

    if(data_->character.sprite && data_->character.spriteVisible)
    {
        const Rectangle absoluteCharBounds = addOffset(data_->character.spriteBounds, myBounds);
        gfx.drawSprite(absoluteCharBounds, data_->character.sprite, data_->character.spriteSettings);
    }

    if(data_->button.sprite && data_->button.spriteVisible)
    {
        const Rectangle absoluteButtonSpriteBounds = addOffset(data_->button.spriteBounds, myBounds);
        gfx.drawSprite(absoluteButtonSpriteBounds, data_->button.sprite, data_->button.spriteSettings);
    }

    if(data_->text[0] != '\0')
    {
        const Rectangle textBounds = {
            .x = myBounds.x + style_.margin.left,
            .y = myBounds.y + style_.margin.top,
            .width = myBounds.width - style_.margin.left - style_.margin.right,
            .height = myBounds.height - style_.margin.top - style_.margin.bottom
        };

        gfx.drawText(textBounds, data_->text, style_.textSettings);
    }

    dialogOptionList_.render(gfx, parentBounds);
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
