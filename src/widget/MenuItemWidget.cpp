#include "widget/MenuItemWidget.h"
#include "core/RDPQGraphics.h"

MenuItemWidget::MenuItemWidget()
    : data_()
    , style_()
    , focused_(false)
    , visible_(true)
    , aButtonPressed_(false)
{
}

MenuItemWidget::~MenuItemWidget()
{
}

void MenuItemWidget::setData(const MenuItemData& data)
{
    data_ = data;
}

void MenuItemWidget::setStyle(const MenuItemStyle& style)
{
    style_ = style;
}

bool MenuItemWidget::isFocused() const
{
    return focused_;
}

void MenuItemWidget::setFocused(bool focused)
{
    focused_ = focused;
}

bool MenuItemWidget::isVisible() const
{
    return visible_;
}

void MenuItemWidget::setVisible(bool visible)
{
    visible_ = visible;
}

Rectangle MenuItemWidget::getBounds() const
{
    return Rectangle{.x = 0, .y = 0, .width = style_.size.width, .height = style_.size.height};
}

void MenuItemWidget::setBounds(const Rectangle& bounds)
{
    // Not relevant: the actual bounds are passed from the VerticalList widget
}

Dimensions MenuItemWidget::getSize() const
{
    return style_.size;
}

bool MenuItemWidget::handleUserInput(const joypad_inputs_t& userInput)
{
    // only handle button release, otherwise you'll be in trouble on scene transitions:
    // the user can't release the button fast enough, so the same press would get handled twice.
    if(userInput.btn.a)
    {
        aButtonPressed_ = true;
        return true;
    }
    else if(aButtonPressed_)
    {
        execute();
        aButtonPressed_ = false;
        return true;
    }
    return false;
}

void MenuItemWidget::render(RDPQGraphics& gfx, const Rectangle& parentBounds)
{
    if(!visible_)
    {
        return;
    }
    Rectangle myBounds = {.x = parentBounds.x, .y = parentBounds.y, .width = style_.size.width, .height = style_.size.height};
    if(style_.background.sprite)
    {
        gfx.drawSprite(myBounds, style_.background.sprite, style_.background.spriteSettings);
    }

    if(style_.icon.sprite)
    {
        const Rectangle iconSpriteBounds = addOffset(style_.icon.spriteBounds, myBounds);
        gfx.drawSprite(iconSpriteBounds, style_.icon.sprite, style_.icon.spriteSettings);
    }

    myBounds.x += style_.leftMargin;
    myBounds.y += style_.topMargin;
    // account for leftMargin and topMargin twice (we also apply it for the rightmargin and bottom)
    myBounds.width -= style_.leftMargin - style_.leftMargin;
    myBounds.height -= style_.topMargin - style_.topMargin;

    gfx.drawText(myBounds, data_.title, (focused_) ? style_.titleFocused : style_.titleNotFocused);
}

void MenuItemWidget::execute()
{
    data_.onConfirmAction(data_.context, data_.itemParam);
}