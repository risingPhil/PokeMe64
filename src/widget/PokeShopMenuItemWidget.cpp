#include "widget/PokeShopMenuItemWidget.h"

PokeShopMenuItem::PokeShopMenuItem()
    : partyIconWidget_()
    , style_({0})
    , data_()
    , priceText_("\0")
    , focused_(false)
    , visible_(true)
    , aButtonPressed_(false)
{
}

PokeShopMenuItem::~PokeShopMenuItem()
{
}

const PokeShopMenuItemData& PokeShopMenuItem::getData() const
{
    return data_;
}

void PokeShopMenuItem::setData(const PokeShopMenuItemData& data)
{
    data_ = data;

    snprintf(priceText_, sizeof(priceText_), "$$%lu", data.price);
    partyIconWidget_.setData(data.iconData);
}

void PokeShopMenuItem::setStyle(const PokeShopMenuItemStyle& style)
{
    style_ = style;

    partyIconWidget_.setBounds(style.icon.bounds);
    partyIconWidget_.setStyle(style.icon.style);
}

bool PokeShopMenuItem::isFocused() const
{
    return focused_;
}

void PokeShopMenuItem::setFocused(bool isFocused)
{
    focused_ = isFocused;
    partyIconWidget_.setFocused(isFocused);
}

bool PokeShopMenuItem::isVisible() const
{
    return visible_;
}

void PokeShopMenuItem::setVisible(bool visible)
{
    visible_ = visible;
}

Rectangle PokeShopMenuItem::getBounds() const
{
    return Rectangle{.x = 0, .y = 0, .width = style_.size.width, .height = style_.size.height};
}

void PokeShopMenuItem::setBounds(const Rectangle& bounds)
{
    // Not relevant: the actual bounds are passed from the VerticalList widget
}

Dimensions PokeShopMenuItem::getSize() const
{
    return style_.size;
}

bool PokeShopMenuItem::handleUserInput(const joypad_inputs_t& userInput)
{
    // We only care about the A-button press/release for this widget
    if(userInput.btn.a)
    {
        aButtonPressed_ = true;
        return true;
    }
    else if(aButtonPressed_)
    {
        aButtonPressed_ = false;
        return execute();
    }

    return false;
}

void PokeShopMenuItem::render(RDPQGraphics& gfx, const Rectangle& parentBounds)
{
    if(!visible_)
    {
        return;
    }
    const Rectangle myBounds = {.x = parentBounds.x, .y = parentBounds.y, .width = style_.size.width, .height = style_.size.height};
    if(style_.background.sprite)
    {
        gfx.drawSprite(myBounds, style_.background.sprite, style_.background.spriteSettings);
    }

    partyIconWidget_.render(gfx, myBounds);

    const Rectangle titleBounds = { myBounds.x + style_.title.bounds.x,
                                    myBounds.y + style_.title.bounds.y,
                                    style_.title.bounds.width,
                                    style_.title.bounds.height };

    gfx.drawText(titleBounds, data_.title, (focused_) ? style_.title.labelFocused : style_.title.labelNotFocused);

    const Rectangle priceBounds = { myBounds.x + style_.price.bounds.x,
                                    myBounds.y + style_.price.bounds.y,
                                    style_.price.bounds.width,
                                    style_.price.bounds.height };

    gfx.drawText(priceBounds, priceText_, (focused_) ? style_.price.labelFocused : style_.price.labelNotFocused);
}

bool PokeShopMenuItem::execute()
{
    if(data_.onConfirmAction)
    {
        data_.onConfirmAction(data_.context, data_.itemParam);
        return true;
    }
    return false;
}