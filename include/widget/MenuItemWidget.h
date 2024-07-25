#ifndef _MENUITEMWIDGET_H
#define _MENUITEMWIDGET_H

#include "widget/IWidget.h"
#include "core/Sprite.h"
#include "core/RDPQGraphics.h"

#include <cstdint>

/**
 * The data struct that will be shown by MenuItemWidget
 */
typedef struct MenuItemData
{
    /**
     * menu item text/title
     */
    const char* title;
    /**
     * function pointer to a callback function that will handle the "confirm" action
     */
    void (*onConfirmAction)(void* context, const void* itemParam);
    /**
     * A user context that will be passed to the onConfirmAction() callback when called
     */
    void* context;

    /**
     * An additional user param which will be passed to the onConfirmAction callback
     */
    const void* itemParam;
} MenuItemData;

/**
 * a style struct that describes the style of the MenuItemWidget
 */
typedef struct MenuItemStyle
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
        /**
         * (optional) icon sprite
         */
        sprite_t* sprite;

        /**
         * RenderSettings that influence how the iconSprite is being rendered
         */
        SpriteRenderSettings spriteSettings;
        
        /**
         * relative bounds of the icon sprite in relation to the MenuItem widget
         */
        Rectangle spriteBounds;
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
} MenuItemStyle;

/**
 * This is a widget created for displaying inside a VerticalList widget to show a menu item
 */
class MenuItemWidget : public IWidget
{
public:
    MenuItemWidget();
    virtual ~MenuItemWidget();

    void setData(const MenuItemData& data);
    void setStyle(const MenuItemStyle& style);

    bool isFocused() const override;
    void setFocused(bool isFocused) override;

    bool isVisible() const override;
    void setVisible(bool visible) override;

    Rectangle getBounds() const override;
    void setBounds(const Rectangle& bounds);
    
    Dimensions getSize() const override;
    
    bool handleUserInput(const joypad_inputs_t& userInput) override;
    void render(RDPQGraphics& gfx, const Rectangle& parentBounds) override;
protected:
    /**
     * Executes the onConfirmAction callback (if any)
     */
    void execute();
private:
    MenuItemData data_;
    MenuItemStyle style_;
    bool focused_;
    bool visible_;
    bool aButtonPressed_;
};

#endif
