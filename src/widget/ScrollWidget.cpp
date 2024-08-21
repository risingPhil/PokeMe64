#include "widget/ScrollWidget.h"
#include "widget/IScrollWindowListener.h"
#include "core/common.h"
#include "scenes/AbstractUIScene.h"
#include "animations/AnimationManager.h"
#include "core/RDPQGraphics.h"

#include <algorithm>

static uint8_t ANALOG_STICK_MIN_THRESHOLD = 30;
static uint8_t ANALOG_STICK_MAX_THRESHOLD = 60;

static FloatVector determineScrollDirection(const joypad_inputs_t& userInput)
{
    // this value should end up giving a semi-normalized vector when used for both x and y coords
    const float DPAD_2_DIRECTIONS_PRESSED_COORD = 0.7071f;
    const float STICK_RANGE_UNIT = 1.f / (ANALOG_STICK_MAX_THRESHOLD - ANALOG_STICK_MIN_THRESHOLD);

    if(userInput.btn.d_left)
    {
        if(userInput.btn.d_up)
        {
            return FloatPoint{ -DPAD_2_DIRECTIONS_PRESSED_COORD, -DPAD_2_DIRECTIONS_PRESSED_COORD};
        }
        else if(userInput.btn.d_down)
        {
            return FloatPoint{ -DPAD_2_DIRECTIONS_PRESSED_COORD, DPAD_2_DIRECTIONS_PRESSED_COORD};
        }
        else
        {
            return FloatPoint{-1.f, 0.f};
        }
    }
    else if(userInput.btn.d_right)
    {
         if(userInput.btn.d_up)
        {
            return FloatPoint{ DPAD_2_DIRECTIONS_PRESSED_COORD, -DPAD_2_DIRECTIONS_PRESSED_COORD};
        }
        else if(userInput.btn.d_down)
        {
            return FloatPoint{ DPAD_2_DIRECTIONS_PRESSED_COORD, DPAD_2_DIRECTIONS_PRESSED_COORD};
        }
        else
        {
            return FloatPoint{1.f, 0.f};
        }       
    }
    else if(userInput.btn.d_up)
    {
        return FloatPoint{0.f, -1.f};
    }
    else if(userInput.btn.d_down)
    {
        return FloatPoint{0.f, 1.f};
    }
    else
    {
        // analog stick
        int8_t absXVal = static_cast<int8_t>(abs(userInput.stick_x));
        int8_t absYVal = static_cast<int8_t>(abs(userInput.stick_y));
        float resultX = 0.f;
        float resultY = 0.f;

        if(absXVal < ANALOG_STICK_MIN_THRESHOLD && absYVal < ANALOG_STICK_MIN_THRESHOLD)
        {
            return FloatPoint{0.f, 0.f};
        }

        // clamp the x and y values to ANALOG_STICK_MAX_THRESHOLD
        if(absXVal > ANALOG_STICK_MIN_THRESHOLD)
        {
            absXVal = (absXVal > ANALOG_STICK_MAX_THRESHOLD) ? ANALOG_STICK_MAX_THRESHOLD : absXVal;
            // remove the dead zone and map the resulting range to 0.f-1.f
            resultX = static_cast<float>(absXVal - ANALOG_STICK_MIN_THRESHOLD) * STICK_RANGE_UNIT;
        }
        if(absYVal > ANALOG_STICK_MIN_THRESHOLD)
        {
            absYVal = (absYVal > ANALOG_STICK_MAX_THRESHOLD) ? ANALOG_STICK_MAX_THRESHOLD : absYVal;
            // remove the dead zone and map the resulting range to 0.f-1.f
            resultY = static_cast<float>(absYVal - ANALOG_STICK_MIN_THRESHOLD) * STICK_RANGE_UNIT;
        }

        if(userInput.stick_x < 0)
        {
            resultX = -resultX;
        }
        if(userInput.stick_y > 0)
        {
            // up in the UI means negative y values
            resultY = -resultY;
        }
        return FloatPoint{resultX, resultY};
    }

    return FloatPoint{0.f, 0.f};
}

MoveScrollWidgetWindowAnimation::MoveScrollWidgetWindowAnimation(ScrollWidget* list)
    : AbstractAnimation(1.f)
    , list_(list)
    , windowStartPoint_({0})
    , windowEndPoint_({0})
{
}

MoveScrollWidgetWindowAnimation::~MoveScrollWidgetWindowAnimation()
{
}

AnimationDistanceTimeFunctionType MoveScrollWidgetWindowAnimation::getDistanceTimeFunctionType() const
{
    return AnimationDistanceTimeFunctionType::LINEAR;
}

uint32_t MoveScrollWidgetWindowAnimation::getDurationInMs() const
{
    return AbstractUIScene::MINIMUM_TIME_BETWEEN_INPUT_EVENTS;
}

void MoveScrollWidgetWindowAnimation::start(const Point& startPoint, const Point& endPoint)
{
    currentTimePos_ = 0.f;
    windowStartPoint_ = startPoint;
    windowEndPoint_ = endPoint;
}

void MoveScrollWidgetWindowAnimation::apply(float pos)
{
    const int px = static_cast<int>(ceil(windowStartPoint_.x + (pos * (windowEndPoint_.x - windowStartPoint_.x))));
    const int py = static_cast<int>(ceil(windowStartPoint_.y + (pos * (windowEndPoint_.y - windowStartPoint_.y))));
    const Point newWindowStart{px, py};
    list_->setWindowStart(newWindowStart);
}

ScrollWidget::ScrollWidget(AnimationManager& animManager)
    : windowAnimation_(this)
    , widgets_()
    , scrollWindowListeners_()
    , style_({0})
    , animManager_(animManager)
    , bounds_({0})
    , windowBounds_({0})
    , visible_(true)
    , focused_(false)
{
    animManager_.add(&windowAnimation_);
}

ScrollWidget::~ScrollWidget()
{
    animManager_.remove(&windowAnimation_);
}

bool ScrollWidget::isFocused() const
{
    return focused_;
}

void ScrollWidget::setFocused(bool isFocused)
{
    focused_ = isFocused;
}

bool ScrollWidget::isVisible() const
{
    return visible_;
}

void ScrollWidget::setVisible(bool visible)
{
    visible_ = visible;
}

Rectangle ScrollWidget::getBounds() const
{
    return bounds_;
}

void ScrollWidget::setBounds(const Rectangle& bounds)
{
    bounds_ = bounds;
}

Dimensions ScrollWidget::getSize() const
{
    return Dimensions{.width = bounds_.width, .height = bounds_.height};
}

void ScrollWidget::setStyle(const ScrollWidgetStyle& style)
{
    style_ = style;
}

void ScrollWidget::addWidget(IWidget* widget)
{
    widgets_.push_back(widget);
    growWindow(widget);
    notifyScrollWindowListeners();
}

void ScrollWidget::removeWidget(IWidget* widget)
{
    auto it = std::find(widgets_.begin(), widgets_.end(), widget);
    if(it != widgets_.end())
    {
        widgets_.erase(it);
    }
    recalculateWindowSize();
}

void ScrollWidget::clearWidgets()
{
    widgets_.clear();
    windowBounds_ = {0};
    notifyScrollWindowListeners();
}

bool ScrollWidget::handleUserInput(const joypad_inputs_t& userInput)
{
    if(!focused_ || style_.scrollStep == 0)
    {
        return false;
    }

    const FloatVector scrollDirection = determineScrollDirection(userInput);
    if(scrollDirection.x == 0.f && scrollDirection.y == 0.f)
    {
        return false;
    }

    const Point windowStartPoint = {.x = windowBounds_.x, .y = windowBounds_.y};
    Point windowEndPoint = {.x = windowBounds_.x, .y = windowBounds_.y};

    windowEndPoint.x += static_cast<int>(ceil(scrollDirection.x * style_.scrollStep));
    windowEndPoint.y += static_cast<int>(ceil(scrollDirection.y * style_.scrollStep));
    if(windowEndPoint.x < 0)
    {
        windowEndPoint.x = 0;
    }
    if(windowEndPoint.y < 0)
    {
        windowEndPoint.y = 0;
    }
    if(windowEndPoint.x + bounds_.width > windowBounds_.width)
    {
        windowEndPoint.x = windowBounds_.width - bounds_.width;
    }
    if(windowEndPoint.y + bounds_.height > windowBounds_.height)
    {
        windowEndPoint.y = windowBounds_.height - bounds_.height;
    }
    windowAnimation_.start(windowStartPoint, windowEndPoint);

    return true;
}

void ScrollWidget::render(RDPQGraphics& gfx, const Rectangle& parentBounds)
{
    if(!visible_)
    {
        return;
    }
    
    const Rectangle myAbsoluteBounds = addOffset(bounds_, parentBounds);

    // we "correct" the x/y coords here because the child widget
    // will use addOffset() to apply its relative bounds (which may have higher xy coords than the width and height of the window) to the parentBounds
    // by doing this correction, we compensate for the window start offset
    const Rectangle absoluteWindowStartBounds = Rectangle{
        .x = myAbsoluteBounds.x - windowBounds_.x, 
        .y = myAbsoluteBounds.y - windowBounds_.y,
        .width = myAbsoluteBounds.width,
        .height = myAbsoluteBounds.height
    };

    // the bounds of the widget instances are all relative to the top of the scroll widget.
    // so is the windowStart_
    // so we need to define a rectangle to define the overlap in order to decide whether a widget needs to be rendered at all.
    const Rectangle relativeWindowRect = {
        .x = windowBounds_.x,
        .y = windowBounds_.y,
        .width = bounds_.width,
        .height = bounds_.height
    };

    const Rectangle prevClip = gfx.getClippingRectangle();
    gfx.setClippingRectangle(myAbsoluteBounds);

    for(IWidget* widget : widgets_)
    {
        if(doRectanglesOverlap(relativeWindowRect, widget->getBounds()))
        {
            widget->render(gfx, absoluteWindowStartBounds);
        }
    }
    gfx.setClippingRectangle(prevClip);
}

void ScrollWidget::setWindowStart(const Point& windowStart)
{
    windowBounds_.x = windowStart.x;
    windowBounds_.y = windowStart.y;

    notifyScrollWindowListeners();
}

void ScrollWidget::registerScrollWindowListener(IScrollWindowListener* listener)
{
    scrollWindowListeners_.push_back(listener);
}

void ScrollWidget::unregisterScrollWindowListener(IScrollWindowListener* listener)
{
    auto it = std::find(scrollWindowListeners_.begin(), scrollWindowListeners_.end(), listener);
    if(it != scrollWindowListeners_.end())
    {
        scrollWindowListeners_.erase(it);
    }
}

void ScrollWidget::growWindow(IWidget* widget)
{
    const Rectangle widgetBounds = widget->getBounds();

    const int widgetMaxX = widgetBounds.x + widgetBounds.width;
    const int widgetMaxY = widgetBounds.y + widgetBounds.height;

    if(widgetMaxX > windowBounds_.width)
    {
        windowBounds_.width = widgetMaxX + style_.marginRight;
    }
    if(widgetMaxY > windowBounds_.height)
    {
        windowBounds_.height = widgetMaxY + style_.marginBottom;
    }
}

void ScrollWidget::recalculateWindowSize()
{
    windowBounds_.width = 0;
    windowBounds_.height = 0;
    for(IWidget* widget : widgets_)
    {
        growWindow(widget);
    }
    notifyScrollWindowListeners();
}

void ScrollWidget::notifyScrollWindowListeners()
{
    const ScrollWindowUpdate update = {
        .scrollWindowRectangle = {.x = windowBounds_.x, .y = windowBounds_.y, .width = bounds_.width, .height = bounds_.height},
        .totalSize = {.width = windowBounds_.width, .height = windowBounds_.height}
    };

    for(IScrollWindowListener* listener : scrollWindowListeners_)
    {
        listener->onScrollWindowChanged(update);
    }
}