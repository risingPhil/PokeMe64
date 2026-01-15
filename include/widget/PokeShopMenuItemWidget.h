#ifndef _POKESHOPMENUITEMWIDGET_H
#define _POKESHOPMENUITEMWIDGET_H

#include "widget/PokemonPartyIconWidget.h"
#include "widget/MenuItemWidget.h"

typedef struct PokeShopMenuItemStyle
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

    struct {
        /**
         * These are the text settings for when the MenuItemWidget is NOT focused by the user
         */
        TextRenderSettings labelNotFocused;
        /**
         * These are the text render settings for when the MenuItemWidget is focused by the user
         */
        TextRenderSettings labelFocused;
        /**
         * @brief the bounds for the price label
         */
        Rectangle bounds;
    } title;

    struct {
        /**
         * These are the text settings for when the MenuItemWidget is NOT focused by the user
         */
        TextRenderSettings labelNotFocused;
        /**
         * These are the text render settings for when the MenuItemWidget is focused by the user
         */
        TextRenderSettings labelFocused;
        /**
         * @brief the bounds for the price label
         */
        Rectangle bounds;
    } price;
} PokeShopMenuItemStyle;

typedef struct PokeShopMenuItemData : public MenuItemData
{
    PokemonPartyIconWidgetData iconData;
    uint32_t price;
} PokeShopMenuItemData;

/**
 * @brief This is a custom MenuItem widget to display distribution event pokemon in a vertical list menu.
 */
class PokeShopMenuItem : public IWidget
{
public:
    PokeShopMenuItem();
    virtual ~PokeShopMenuItem();

    const PokeShopMenuItemData& getData() const;

    void setData(const PokeShopMenuItemData& data);
    void setStyle(const PokeShopMenuItemStyle& style);

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
    PokeShopMenuItemStyle style_;
    PokeShopMenuItemData data_;
    char priceText_[10];
    bool focused_;
    bool visible_;
    bool aButtonPressed_;
};

#endif