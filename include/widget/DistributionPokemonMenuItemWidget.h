#ifndef _DISTRIBUTIONPOKEMONMENUITEMWIDGET_H
#define _DISTRIBUTIONPOKEMONMENUITEMWIDGET_H

#include "widget/PokemonPartyIconWidget.h"
#include "widget/MenuItemWidget.h"

typedef struct DistributionPokemonMenuItemStyle
{
    /**
     * width and height for the MenuItemWidget
     */
    Dimensions size;
    struct {
        /**
         * (optional) background sprite
         */
        sprite_t* sprite;
        /*
        * RenderSettings that influence how the backgroundSprite is
        * being rendered
        */
        SpriteRenderSettings spriteSettings;
    } background;

    struct {
        PokemonPartyIconWidgetStyle style;
        Rectangle bounds;
    } icon;

    /**
     * These are the text settings for when the MenuItemWidget is NOT focused by the user
     */
    TextRenderSettings titleNotFocused;
    /**
     * These are the text render settings for when the MenuItemWidget is focused by the user
     */
    TextRenderSettings titleFocused;
    /**
     * Offset to indicate how far from the left we need to start rendering the title text
     */
    uint16_t leftMargin;
    /**
     * Offset to indicate how far from the top we need to start rendering the title text
     */
    uint16_t topMargin;
} DistributionPokemonMenuItemStyle;

typedef struct DistributionPokemonMenuItemData : public MenuItemData
{
    PokemonPartyIconWidgetData iconData;
} DistributionPokemonMenuItemData;

/**
 * @brief This is a custom MenuItem widget to display distribution event pokemon in a vertical list menu.
 */
class DistributionPokemonMenuItem : public IWidget
{
public:
    DistributionPokemonMenuItem();
    virtual ~DistributionPokemonMenuItem();

    const DistributionPokemonMenuItemData& getData() const;

    void setData(const DistributionPokemonMenuItemData& data);
    void setStyle(const DistributionPokemonMenuItemStyle& style);

    bool isFocused() const override;
    void setFocused(bool isFocused) override;

    bool isVisible() const override;
    void setVisible(bool visible) override;

    Rectangle getBounds() const override;
    void setBounds(const Rectangle& bounds) override;

    Dimensions getSize() const override;

    bool handleUserInput(const joypad_inputs_t& userInput) override;

    void render(RDPQGraphics& gfx, const Rectangle& parentBounds) override;
protected:
    /**
     * Executes the onConfirmAction callback (if any)
     */
    bool execute();
private:
    PokemonPartyIconWidget partyIconWidget_;
    DistributionPokemonMenuItemStyle style_;
    DistributionPokemonMenuItemData data_;
    bool focused_;
    bool visible_;
    bool aButtonPressed_;
};

#endif