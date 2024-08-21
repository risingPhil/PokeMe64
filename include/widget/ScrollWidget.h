#ifndef _SCROLLWIDGET_H
#define _SCROLLWIDGET_H

#include "widget/IWidget.h"
#include "animations/IAnimation.h"
#include <vector>

class IScrollWindowListener;
class ScrollWidget;
class AnimationManager;

typedef std::vector<IWidget*> WidgetList;
typedef std::vector<IScrollWindowListener*> ScrollWindowListenerList;

typedef struct ScrollWidgetStyle
{
    // it defines the amount of pixels we should scroll when the analog stick or dpad is used.
    uint8_t scrollStep;
    // whitespace to leave at the end of the scroll window (horizontal)
    uint16_t marginRight;
    // whitespace to leave at the end of the scroll window (vertical)
    uint16_t marginBottom;
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
protected:
    void apply(float pos) override;
private:
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

    void registerScrollWindowListener(IScrollWindowListener* listener);
    void unregisterScrollWindowListener(IScrollWindowListener* listener);
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

    void notifyScrollWindowListeners();

    MoveScrollWidgetWindowAnimation windowAnimation_;
    WidgetList widgets_;
    ScrollWindowListenerList scrollWindowListeners_;
    ScrollWidgetStyle style_;
    AnimationManager& animManager_;
    Rectangle bounds_;
    Rectangle windowBounds_;
    bool visible_;
    bool focused_;
};

#endif