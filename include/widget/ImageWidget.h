#ifndef _IMAGEWIDGET_H
#define _IMAGEWIDGET_H

#include "widget/IWidget.h"
#include "core/RDPQGraphics.h"
#include "core/Sprite.h"

typedef struct ImageWidgetStyle
{
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
        bool visibleWhenNotFocused;
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
    } image;
} ImageWidgetStyle;

/**
 * @brief This widget is a way to render an image, but in widget form.
 * That allows it to get added to other widgets, such as a VerticalListWidget
 */
class ImageWidget : public IWidget
{
public:
    ImageWidget();
    virtual ~ImageWidget();

    const ImageWidgetStyle& getStyle() const;
    void setStyle(const ImageWidgetStyle& style);

    bool isFocused() const override;
    void setFocused(bool isFocused) override;

    bool isVisible() const override;
    void setVisible(bool visible) override;

    Rectangle getBounds() const override;
    void setBounds(const Rectangle& bounds) override;
    Dimensions getSize() const override;

    void setConfirmAction(void (*onConfirmAction)(void*), void* context);

    bool handleUserInput(const joypad_inputs_t& userInput) override;
    void render(RDPQGraphics& gfx, const Rectangle& parentBounds) override;
protected:
private:
    ImageWidgetStyle style_;
    Rectangle bounds_;
    void (*onConfirmAction_)(void*);
    void* onConfirmActionContext_;
    bool focused_;
    bool visible_;
    bool aButtonPressed_;
};

#endif