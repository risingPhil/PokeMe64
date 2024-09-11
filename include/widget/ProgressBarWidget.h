#ifndef _PROGRESSBARWIDGET_H
#define _PROGRESSBARWIDGET_H

#include "widget/IWidget.h"
#include "core/Sprite.h"
#include "core/RDPQGraphics.h"

typedef struct ProgressBarWidgetStyle
{
    struct{
        sprite_t* sprite;
        SpriteRenderSettings renderSettings;
    } background;
    struct{
        struct{
            int left;
            int right;
            int top;
            int bottom;
        } margin;
        color_t color;
        sprite_t* sprite;
        SpriteRenderSettings spriteSettings;
    } bar;
    TextRenderSettings textSettings;
} ProgressBarWidgetStyle;

class ProgressBarWidget : public IWidget
{
public:
    ProgressBarWidget();
    virtual ~ProgressBarWidget();

    /**
     * Sets the current progress. Should be a value in the [0.0 - 1.0] interval
     */
    void setProgress(double progress);

    /**
     * Sets the ProgressBarWidget style
     */
    void setStyle(const ProgressBarWidgetStyle& style);

    /**
     * @brief Returns whether the widget is currently focused
     */
    bool isFocused() const override;

    /**
     * @brief Sets whether the widget is currently focused
     *
     */
    void setFocused(bool isFocused) override;

    /**
     * @brief Returns whether the widget is currently visible
     */
    bool isVisible() const override;

    /**
     * @brief Changes the visibility of the widget
     */
    void setVisible(bool visible) override;

    /**
     * @brief Returns the current (relative) bounds of the widget
     */
    Rectangle getBounds() const override;

    /**
     * @brief Changes the current (relative) bounds of the widget
     */
    void setBounds(const Rectangle& bounds) override;

    /**
     * @brief Returns the size (width/height) of the widget
     */
    Dimensions getSize() const override;

    /**
     * @brief Handles user input
     *
     * For button presses, it is advised to track button release situations instead of 
     * button presses for executing an action. Otherwise the key press might be handled again immediately
     * in the next scene/widget because the user wouldn't have had the time to actually release the key.
     */
    bool handleUserInput(const joypad_inputs_t& userInput) override;

    /**
     * @brief Renders the widget
     *
     * @param gfx The graphics instance that must be used to render the widget
     * @param parentBounds The bounds of the parent widget or scene. You must add the x,y offset of your own bounds 
     * to the parentBounds to get the absolute bounds for rendering.
     *
     * Getting the parentBounds as an argument of this function was done because a parent widget may be 
     * animated or change positions independent of the child widget. But when the parent widget moves, the child must as well!
     */
    void render(RDPQGraphics& gfx, const Rectangle& parentBounds) override;
protected:
private:
    ProgressBarWidgetStyle style_;
    bool visible_;
    Rectangle bounds_;
    double progress_;
    char textBuffer_[5];
};

#endif