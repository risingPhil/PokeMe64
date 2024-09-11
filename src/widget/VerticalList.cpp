#include "widget/VerticalList.h"
#include "widget/IWidget.h"
#include "widget/IFocusListener.h"
#include "widget/IScrollWindowListener.h"
#include "core/RDPQGraphics.h"
#include "animations/AnimationManager.h"
#include "core/DragonUtils.h"

#include <cstddef>
#include <algorithm>
#include <cmath>

static bool isWidgetInsideWindow(const Rectangle& widgetBounds, uint32_t listHeight, uint32_t windowStartY)
{
    const int correctedY = widgetBounds.y - windowStartY;
    // we want to have the entire widget in the view, so in a vertical list, that means the bottom of the widget
    // needs to be inside the window
//    debugf("%s: bounds [%d, %d, %d, %d], listHeight %lu, windowStartY %lu, correctedY %d\n", __FUNCTION__, widgetBounds.x, widgetBounds.y, widgetBounds.width, widgetBounds.height, listHeight, windowStartY, correctedY);
    return (correctedY >= 0) && (correctedY + widgetBounds.height <= static_cast<int>(listHeight));
}

static int32_t getVerticalWindowScrollNeededToMakeWidgetFullyVisible(const Rectangle& widgetBounds, uint32_t listHeight, uint32_t windowStartY)
{
    if(widgetBounds.y < static_cast<int32_t>(windowStartY))
    {
        return widgetBounds.y - static_cast<int32_t>(windowStartY);
    }

    const int32_t widgetEndY = widgetBounds.y + widgetBounds.height;
    const int32_t listEndY = static_cast<int32_t>(windowStartY + listHeight);
    if(widgetEndY > listEndY)
    {
        return widgetEndY - listEndY;
    }
    return 0;
}

/**
 * @brief The widgetBoundsList_ contains the coordinates of the widgets starting from the top of the list
 * it assumes there's no such thing as a view window and those coordinates do not take the VerticalList bounds into account.
 * They just start at x=0, y=0 and keep increasing y until the last widget.
 * 
 * But this function maps these coordinates to the actual absolute coordinates on screen
 * by taking the view window y coordinate into account (which also doesn't take the VerticalList bounds into account)
 * and adding the 
 * 
 * @param widgetBounds The bounds of the widget from the widgetBoundList_
 * @param windowStartY the start y position of the view window in the widgetBoundList_ coordinate system
 * @param widgetTopX The absolute left top corner x position from the widget where the first item is supposed to start
 * @param widgetTopY The absolute left top corner y position from the widget where the first item is supposed to start
 * @return const Rectangle 
 */
static const Rectangle calculateListWidgetBounds(const Rectangle& widgetBounds, uint32_t windowStartY, int widgetTopX, int widgetTopY)
{
    return {
        .x = widgetTopX + widgetBounds.x,
        .y = widgetTopY + widgetBounds.y - static_cast<int>(windowStartY),
        .width = widgetBounds.width,
        .height = widgetBounds.height
    };
}

static uint32_t getInnerListHeight(const Rectangle& listBounds, int marginTop, int marginBottom)
{
    return listBounds.height - marginTop - marginBottom;
}

MoveVerticalListWindowAnimation::MoveVerticalListWindowAnimation(VerticalList* list)
    : AbstractAnimation(1.f) // start in the finished state by specifying the 1.f end pos initially
    , list_(list)
    , windowStartY_(0)
    , windowEndY_(0)
{
}

MoveVerticalListWindowAnimation::~MoveVerticalListWindowAnimation()
{
}

AnimationDistanceTimeFunctionType MoveVerticalListWindowAnimation::getDistanceTimeFunctionType() const
{
    return AnimationDistanceTimeFunctionType::EASE_IN_EASE_OUT;
}

uint32_t MoveVerticalListWindowAnimation::getDurationInMs() const
{
    return 250;
}

void MoveVerticalListWindowAnimation::start(uint32_t windowStartY, uint32_t windowEndY)
{
    currentTimePos_ = 0.f;
    windowStartY_ = static_cast<int32_t>(windowStartY);
    windowEndY_ = static_cast<int32_t>(windowEndY);
}

void MoveVerticalListWindowAnimation::apply(float pos)
{
    // we could need a negative value here if windowEndY < windowStartY. But our member windowEndY is unsigned.
    const uint32_t newWindowStart = static_cast<uint32_t>(ceilf(windowStartY_ + (pos * (windowEndY_ - windowStartY_))));
    list_->setViewWindowStartY(newWindowStart);
}

VerticalList::VerticalList(AnimationManager& animationManager)
    : moveWindowAnimation_(this)
    , widgetList_()
    , widgetBoundsList_()
    , focusListeners_()
    , scrollWindowListeners_()
    , listStyle_({0})
    , bounds_({0})
    , windowMinY_(0)
    , focusedWidgetIndex_(0)
    , animManager_(animationManager)
    , focused_(false)
    , visible_(true)
{
    animManager_.add(&moveWindowAnimation_);
}

VerticalList::~VerticalList()
{
    animManager_.remove(&moveWindowAnimation_);
}

bool VerticalList::focusNext()
{
    FocusChangeStatus changeStatus;
    if(focusedWidgetIndex_ + 1 >= widgetList_.size())
    {
        return false;
    }
    // finish previous animation first (skip it) to ensure windowMinY_ is set correctly
    moveWindowAnimation_.skipToEnd();

    changeStatus.prevFocus = widgetList_[focusedWidgetIndex_];
    widgetList_[focusedWidgetIndex_]->setFocused(false);
    
    ++focusedWidgetIndex_;
    
    changeStatus.curFocus = widgetList_[focusedWidgetIndex_];
    changeStatus.focusBounds = calculateListWidgetBounds(widgetBoundsList_[focusedWidgetIndex_], windowMinY_, bounds_.x + listStyle_.margin.left, bounds_.y + listStyle_.margin.top);
    widgetList_[focusedWidgetIndex_]->setFocused(focused_);

    const int32_t scrollAmountY = scrollWindowToFocusedWidget();
    changeStatus.focusBounds.y -= scrollAmountY;

    notifyFocusListeners(changeStatus);

    return true;
}

bool VerticalList::focusPrevious()
{
    FocusChangeStatus changeStatus;
    if(focusedWidgetIndex_ == 0)
    {
        return false;
    }

    // finish previous animation first (skip it) to ensure windowMinY_ is set correctly
    moveWindowAnimation_.skipToEnd();

    changeStatus.prevFocus = widgetList_[focusedWidgetIndex_];
    widgetList_[focusedWidgetIndex_]->setFocused(false);

    --focusedWidgetIndex_;

    changeStatus.curFocus = widgetList_[focusedWidgetIndex_];
    changeStatus.focusBounds = calculateListWidgetBounds(widgetBoundsList_[focusedWidgetIndex_], windowMinY_, bounds_.x + listStyle_.margin.left, bounds_.y + listStyle_.margin.top);
    widgetList_[focusedWidgetIndex_]->setFocused(focused_);
    
    const int32_t scrollAmountY = scrollWindowToFocusedWidget();
    changeStatus.focusBounds.y -= scrollAmountY;

    notifyFocusListeners(changeStatus);

    return true;
}

void VerticalList::addWidget(IWidget *widget)
{
    const Dimensions widgetSize = widget->getSize();
    widgetList_.push_back(widget);

    if (widgetList_.size() == 1)
    {
        widgetBoundsList_.push_back(Rectangle{.x = 0, .y = 0, .width = widgetSize.width, .height = widgetSize.height});
        widget->setFocused(focused_);

        FocusChangeStatus changeStatus = {
            .focusBounds = calculateListWidgetBounds(widgetBoundsList_[focusedWidgetIndex_], windowMinY_, bounds_.x + listStyle_.margin.left, bounds_.y + listStyle_.margin.top),
            .prevFocus = nullptr,
            .curFocus = widget
        };
        notifyFocusListeners(changeStatus);
    }
    else
    {
        const Rectangle& lastWidgetBounds = widgetBoundsList_.back();
        widgetBoundsList_.push_back(Rectangle{.x = 0, .y = lastWidgetBounds.y + lastWidgetBounds.height + listStyle_.verticalSpacingBetweenWidgets, .width = widgetSize.width, .height = widgetSize.height});
    }
    autoGrowBounds();
    notifyScrollWindowListeners();
}

void VerticalList::removeWidget(IWidget* widget)
{
    const auto listSize = widgetList_.size();
    for(uint16_t i = 0; i < listSize; ++i)
    {
        if(widgetList_[i] != widget)
        {
            continue;
        }
        //found the widget
        // if the widget -that we're about to remove- is focused, we need to move the focus to a different widget
        if(widgetList_[i]->isFocused() && listSize > 1)
        {
            // if the to-be-removed widget is the last one in the list, we need to focus the one before it.
            // if it's not, we'll focus the next one
            uint16_t newFocusIndex;
            if(i < (listSize - 1))
            {
                newFocusIndex = i + 1;
                // the next list item will move to the current position after the erase() call
                focusedWidgetIndex_ = i;
            }
            else
            {
                newFocusIndex = i - 1;
                focusedWidgetIndex_ = newFocusIndex;
            }
            widgetList_[newFocusIndex]->setFocused(true);
        }

        widgetList_.erase(widgetList_.begin() + i);
        widgetBoundsList_.erase(widgetBoundsList_.begin() + i);
        rebuildLayout();
        break;
    }
}

void VerticalList::clearWidgets()
{
    widgetList_.clear();
    widgetBoundsList_.clear();
    focusedWidgetIndex_ = 0;
    windowMinY_ = 0;
    notifyScrollWindowListeners();
}

VerticalListStyle& VerticalList::getStyle()
{
    return listStyle_;
}

void VerticalList::setStyle(const VerticalListStyle& style)
{
    listStyle_ = style;
    rebuildLayout();
}

void VerticalList::setViewWindowStartY(uint32_t windowStartY)
{
    windowMinY_ = windowStartY;
    notifyScrollWindowListeners();
}

bool VerticalList::isFocused() const
{
    return focused_;
}

void VerticalList::setFocused(bool isFocused)
{
    focused_ = isFocused;
    if(widgetList_.empty())
    {
        return;
    }
    widgetList_[focusedWidgetIndex_]->setFocused(focused_);

    FocusChangeStatus changeStatus = {
        .focusBounds = calculateListWidgetBounds(widgetBoundsList_[focusedWidgetIndex_], windowMinY_, bounds_.x + listStyle_.margin.left, bounds_.y + listStyle_.margin.top)
    };

    if(isFocused)
    {
        changeStatus.prevFocus = nullptr;
        changeStatus.curFocus = widgetList_[focusedWidgetIndex_];
    }
    else 
    {
        changeStatus.prevFocus = widgetList_[focusedWidgetIndex_];
        changeStatus.curFocus = nullptr;
    }

    notifyFocusListeners(changeStatus);
}

bool VerticalList::isVisible() const
{
    return visible_;
}

void VerticalList::setVisible(bool visible)
{
    visible_ = visible;
}

Rectangle VerticalList::getBounds() const
{
    return bounds_;
}

void VerticalList::setBounds(const Rectangle &bounds)
{
    bounds_ = bounds;
    rebuildLayout();
}

Dimensions VerticalList::getSize() const
{
    return getDimensions(bounds_);
}

bool VerticalList::handleUserInput(const joypad_inputs_t& userInput)
{
    if(widgetList_.empty())
    {
        return false;
    }
    
    const bool hasFocusedWidgetHandledInput = widgetList_[focusedWidgetIndex_]->handleUserInput(userInput);

    if(hasFocusedWidgetHandledInput)
    {
        return hasFocusedWidgetHandledInput;
    }

    const UINavigationDirection navDirection = determineUINavigationDirection(userInput, NavigationInputSourceType::BOTH);

    if(navDirection == UINavigationDirection::UP)
    {
        if(focusedWidgetIndex_ < 1)
        {
            return false;
        }
        return focusPrevious();
    }
    else if(navDirection == UINavigationDirection::DOWN)
    {
        if(focusedWidgetIndex_ == widgetList_.size() - 1)
        {
            return false;
        }
        return focusNext();
    }
    
    return false;
}

void VerticalList::render(RDPQGraphics& gfx, const Rectangle& parentBounds)
{
    if(!visible_)
    {
        return;
    }

    const uint32_t innerListHeight = getInnerListHeight(bounds_, listStyle_.margin.top, listStyle_.margin.bottom);
    uint32_t i;
    const Rectangle myBounds = addOffset(bounds_, parentBounds);
    const int topX = myBounds.x + listStyle_.margin.left;
    const int topY = myBounds.y + listStyle_.margin.top;

    // store previous clipping rectangle to restore later
//    const Rectangle prevClipRect = gfx.getClippingRectangle();

//    gfx.setClippingRectangle(myBounds);

    // render the background first, if any.
    if(listStyle_.background.sprite)
    {
        gfx.drawSprite(myBounds, listStyle_.background.sprite, listStyle_.background.spriteSettings);
    }

    if(widgetList_.empty())
    {
        // restore previous clipping rectangle
//        gfx.setClippingRectangle(prevClipRect);
        return;
    }

    // find the first visible item
    for(i = 0; i < widgetList_.size(); ++i)
    {
        if(isWidgetInsideWindow(widgetBoundsList_[i], innerListHeight, windowMinY_))
        {
            // found it
            break;
        }
    }

    if(i > widgetList_.size() - 1)
    {
        // no items to be rendered
        // restore previous clipping rectangle
 //       gfx.setClippingRectangle(prevClipRect);
        return;
    }

    // now start rendering the widgets
//    debugf("[VerticalList]: %s: now start rendering the widgets at i=%lu, widgetList_ size %lu, widgetBoundsList_ size %lu\r\n", __FUNCTION__, i, static_cast<uint32_t>(widgetList_.size()), static_cast<uint32_t>(widgetBoundsList_.size()));
    do
    {
        const Rectangle listBounds = calculateListWidgetBounds(widgetBoundsList_[i], windowMinY_, topX, topY);

//        debugf("[VerticalList]: widget %lu: %p\r\n", i, widgetList_[i]);
        widgetList_[i]->render(gfx, listBounds);

        ++i;
//        debugf("[VerticalList]: next iteration %lu\r\n", i);
    } while ((i < widgetList_.size()) && isWidgetInsideWindow(widgetBoundsList_[i], innerListHeight, windowMinY_));
    
//    debugf("end loop\r\n");
    // restore previous clipping rectangle
//    gfx.setClippingRectangle(prevClipRect);
}

void VerticalList::registerFocusListener(IFocusListener* focusListener)
{
    focusListeners_.push_back(focusListener);
}

void VerticalList::unregisterFocusListener(IFocusListener* focusListener)
{
    auto it = std::find(focusListeners_.begin(), focusListeners_.end(), focusListener);
    if(it != focusListeners_.end())
    {
        focusListeners_.erase(it);
    }
}

void VerticalList::registerScrollWindowListener(IScrollWindowListener* listener)
{
    scrollWindowListeners_.push_back(listener);
}

void VerticalList::unregisterScrollWindowListener(IScrollWindowListener* listener)
{
    auto it = std::find(scrollWindowListeners_.begin(), scrollWindowListeners_.end(), listener);
    if(it != scrollWindowListeners_.end())
    {
        scrollWindowListeners_.erase(it);
    }
}

void VerticalList::notifyScrollWindowListeners()
{
    int totalHeight;

    if(widgetList_.empty())
    {
        totalHeight = 0;
    }
    else
    {
        const Rectangle& lastWidgetBounds = widgetBoundsList_.back();
        totalHeight = lastWidgetBounds.y + lastWidgetBounds.height;   
    }

    // calculate the size of the scroll panel with the margins subtracted
    const uint32_t innerListHeight = getInnerListHeight(bounds_, listStyle_.margin.top, listStyle_.margin.bottom);

    const ScrollWindowUpdate update = {
        .scrollWindowRectangle = {.x = 0, .y = static_cast<int>(windowMinY_), .width = bounds_.width, .height = static_cast<int>(innerListHeight)},
        .totalSize = {.width = bounds_.width, .height = totalHeight}
    };

    for(IScrollWindowListener* listener : scrollWindowListeners_)
    {
        listener->onScrollWindowChanged(update);
    }
}

void VerticalList::rebuildLayout()
{
    Rectangle focusedWidgetBounds;
    IWidget* focusedWidget;
    int lastWidgetEndY = 0;
    if(widgetList_.empty())
    {
        return;
    }

    // reset of the widgets
    for (size_t i = 0; i < widgetList_.size(); ++i)
    {
        const Dimensions widgetSize = widgetList_[i]->getSize();

        widgetBoundsList_[i] = {.x = 0, .y = lastWidgetEndY, .width = widgetSize.width, .height = widgetSize.height};
        lastWidgetEndY += widgetSize.height + listStyle_.verticalSpacingBetweenWidgets;
    }
    
    if(!widgetList_.empty())
    {
        focusedWidgetBounds = widgetBoundsList_[focusedWidgetIndex_];
        focusedWidget = widgetList_[focusedWidgetIndex_];
    }
    else
    {
        // HACK: safety net in case the list is empty (to avoid an out-of-bounds list access crash)
        // the use of windowMinY_ here is to compensate for what calculateListWidgetBounds is calculating
        focusedWidgetBounds = Rectangle{.x = 0, .y = static_cast<int>(windowMinY_), .width = 0, .height = 0};
        focusedWidget = nullptr;
    }
    
    const FocusChangeStatus changeStatus = {
        .focusBounds = calculateListWidgetBounds(focusedWidgetBounds, windowMinY_, bounds_.x + listStyle_.margin.left, bounds_.y + listStyle_.margin.top),
        .curFocus = focusedWidget
    };

    notifyFocusListeners(changeStatus);
    notifyScrollWindowListeners();
}

int32_t VerticalList::scrollWindowToFocusedWidget()
{
    //TODO: the new widget is only visible after the scroll is finished.
    // the reason is the use of isWidgetInsideWindow() inside the render() function to cull entries from the render window.
    // We could potentially eliminate this by expanding the check to allow a partially visible entry to be rendered.
    // But for my goals, this is currently not needed, so I claim YAGNI for now.
    const uint32_t innerListHeight = getInnerListHeight(bounds_, listStyle_.margin.top, listStyle_.margin.bottom);
    const int32_t windowScrollYNeeded = getVerticalWindowScrollNeededToMakeWidgetFullyVisible(widgetBoundsList_[focusedWidgetIndex_], innerListHeight, windowMinY_);
    if(windowScrollYNeeded != 0)
    {
        moveWindow(windowScrollYNeeded);
    }
    return windowScrollYNeeded;
}

void VerticalList::moveWindow(int32_t yAmount)
{
    moveWindowAnimation_.start(windowMinY_, windowMinY_ + yAmount);
}

void VerticalList::notifyFocusListeners(const FocusChangeStatus& status)
{
    for(IFocusListener* listener : focusListeners_)
    {
        listener->focusChanged(status);
    }
}

void VerticalList::autoGrowBounds()
{
    if(!listStyle_.autogrow.enabled || widgetList_.empty())
    {
        return;
    }

    const Rectangle& lastWidgetBounds = widgetBoundsList_.back();
    int heightToConsider = lastWidgetBounds.y + lastWidgetBounds.height + listStyle_.margin.top + listStyle_.margin.bottom;
    if(listStyle_.autogrow.maxHeight && listStyle_.autogrow.maxHeight < heightToConsider)
    {
        heightToConsider = listStyle_.autogrow.maxHeight;
    }

    if(listStyle_.autogrow.shouldGrowUpWards)
    {
        const int heightDiff = abs(heightToConsider - bounds_.height);
        bounds_.y -= heightDiff;
    }

    bounds_.height = heightToConsider;
}