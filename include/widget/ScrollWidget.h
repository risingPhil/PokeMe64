#ifndef _SCROLLWIDGET_H
#define _SCROLLWIDGET_H

#include "widget/IWidget.h"
#include "animations/IAnimation.h"
#include <vector>

typedef std::vector<IWidget*> WidgetList;

class ScrollWidget;
class AnimationManager;

typedef struct ScrollWidgetStyle
{
    // it defines the amount of pixels we should scroll when the analog stick or dpad is used.
    uint8_t scrollStep;
} ScrollWidgetStyle;

/**
 * @brief This Animation implementation is used internal in ScrollWidget
 * to move the "view window"'s x and y-coordinates.
 * 
 * It's specific to the ScrollWidget
 */
class MoveScrollWidgetWindowAnimation : public AbstractAnimation
{
public:
    MoveScrollWidgetWindowAnimation(ScrollWidget* list);
    virtual ~MoveScrollWidgetWindowAnimation();

    AnimationDistanceTimeFunctionType getDistanceTimeFunctionType() const override;

    uint32_t getDurationInMs() const override;

    void start(const Point& startPoint, const Point& endPoint);
    void extend(const Point& endPoint);
protected:
    void apply(float pos) override;
private:
    Point calculatePoint(float pos);

    ScrollWidget* list_;
    Point windowStartPoint_;
    Point windowEndPoint_;
};

/**
 * @brief This is a widget that contains other widgets and can scroll through it.
 * It differs from VerticalList in that ScrollWidget manages "unstructured" widgets instead of just a list in a certain direction.
 * 
 * But this scroll widget does not allow you to select a widget and/or handle events on it. Every widget added will remain unfocused and won't handle keys
 * display only.
 * 
 */
class ScrollWidget : public IWidget
{
public:
    ScrollWidget(AnimationManager& animManager);
    virtual ~ScrollWidget();

    bool isFocused() const override;
    void setFocused(bool isFocused) override;

    bool isVisible() const override;
    void setVisible(bool visible) override;

    Rectangle getBounds() const override;
    void setBounds(const Rectangle& bounds) override;
    Dimensions getSize() const override;

    void setStyle(const ScrollWidgetStyle& style);

    void addWidget(IWidget* widget);
    void removeWidget(IWidget* widget);
    void clearWidgets();

    bool handleUserInput(const joypad_inputs_t& userInput) override;
    void render(RDPQGraphics& gfx, const Rectangle& parentBounds) override;

    /**
     * @brief Sets the view window start xy coords.
     * This is used by MoveScrollWidgetWindowAnimation
     */
    void setWindowStart(const Point& windowStart);

    /**
     * @brief Indicates the percentage at which the current window position is at horizontally
     *  in relation to the entire window
     */
    float getWindowProgressX() const;
    /**
     * @brief Indicates the percentage at which the current window position is at vertically
     *  in relation to the entire window
     */
    float getWindowProgressY() const;
protected:
private:
    /**
     * @brief This function will grow the maximum dimensions of the window
     * based on the specified IWidget
     */
    void growWindow(IWidget* widget);
    /**
     * @brief Recalculates the entire width and height of the view window
     * This is necessary when removing a widget
     */
    void recalculateWindowSize();

    MoveScrollWidgetWindowAnimation windowAnimation_;
    WidgetList widgets_;
    ScrollWidgetStyle style_;
    AnimationManager& animManager_;
    Rectangle bounds_;
    Rectangle windowBounds_;
    bool visible_;
    bool focused_;
};

#endif