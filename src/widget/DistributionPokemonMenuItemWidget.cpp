#include "widget/DistributionPokemonMenuItemWidget.h"

DistributionPokemonMenuItem::DistributionPokemonMenuItem()
    : partyIconWidget_()
    , style_({0})
    , data_()
    , focused_(false)
    , visible_(true)
    , aButtonPressed_(false)
{
}

DistributionPokemonMenuItem::~DistributionPokemonMenuItem()
{
}

const DistributionPokemonMenuItemData& DistributionPokemonMenuItem::getData() const
{
    return data_;
}

void DistributionPokemonMenuItem::setData(const DistributionPokemonMenuItemData& data)
{
    data_ = data;

    partyIconWidget_.setData(data.iconData);
}

void DistributionPokemonMenuItem::setStyle(const DistributionPokemonMenuItemStyle& style)
{
    style_ = style;

    partyIconWidget_.setBounds(style.icon.bounds);
    partyIconWidget_.setStyle(style.icon.style);
}

bool DistributionPokemonMenuItem::isFocused() const
{
    return focused_;
}

void DistributionPokemonMenuItem::setFocused(bool isFocused)
{
    focused_ = isFocused;
    partyIconWidget_.setFocused(isFocused);
}

bool DistributionPokemonMenuItem::isVisible() const
{
    return visible_;
}

void DistributionPokemonMenuItem::setVisible(bool visible)
{
    visible_ = visible;
}

Rectangle DistributionPokemonMenuItem::getBounds() const
{
    return Rectangle{.x = 0, .y = 0, .width = style_.size.width, .height = style_.size.height};
}

void DistributionPokemonMenuItem::setBounds(const Rectangle& bounds)
{
    // Not relevant: the actual bounds are passed from the VerticalList widget
}

Dimensions DistributionPokemonMenuItem::getSize() const
{
    return style_.size;
}

bool DistributionPokemonMenuItem::handleUserInput(const joypad_inputs_t& userInput)
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
        aButtonPressed_ = false;
        return execute();
    }
    return false;
}

void DistributionPokemonMenuItem::render(RDPQGraphics& gfx, const Rectangle& parentBounds)
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

    partyIconWidget_.render(gfx, myBounds);

    myBounds.x += style_.leftMargin;
    myBounds.y += style_.topMargin;
    // account for leftMargin and topMargin twice (we also apply it for the rightmargin and bottom)
    myBounds.width -= style_.leftMargin;
    myBounds.width -= style_.leftMargin;
    myBounds.height -= style_.topMargin - style_.topMargin;

    gfx.drawText(myBounds, data_.title, (focused_) ? style_.titleFocused : style_.titleNotFocused);
}

bool DistributionPokemonMenuItem::execute()
{
    if(data_.onConfirmAction)
    {
        data_.onConfirmAction(data_.context, data_.itemParam);
        return true;
    }
    return false;
}