#include "widget/IScrollWindowListener.h"
#include <libdragon.h>

bool canScrollTo(const ScrollWindowUpdate& info, UINavigationDirection direction)
{
    switch(direction)
    {
        case UINavigationDirection::UP:
            return (info.scrollWindowRectangle.y > 0);
        case UINavigationDirection::RIGHT:
            return (info.scrollWindowRectangle.x + info.scrollWindowRectangle.width < info.totalSize.width);
        case UINavigationDirection::DOWN:
            return (info.scrollWindowRectangle.y + info.scrollWindowRectangle.height < info.totalSize.height);
        case UINavigationDirection::LEFT:
            return (info.scrollWindowRectangle.x > 0);
        default:
            return false;
    }
}

IScrollWindowListener::~IScrollWindowListener()
{
}

TestScrollWindowListener::~TestScrollWindowListener()
{
}

void TestScrollWindowListener::onScrollWindowChanged(const ScrollWindowUpdate &update)
{
    bool canScroll[4];
    for(int i=0; i < 4; ++i)
    {
        canScroll[i] = canScrollTo(update, static_cast<UINavigationDirection>(i));
    }
    debugf("[TestScrollWindowListener]:%s:%p: scrollWindowRectangle: [%d, %d, %d, %d], totalSize: [%d, %d], scrollUp: %d, scrollRight %d, scrollDown %d, scrollLeft %d\r\n", __FUNCTION__, this, update.scrollWindowRectangle.x, update.scrollWindowRectangle.y, update.scrollWindowRectangle.width, update.scrollWindowRectangle.height, update.totalSize.width, update.totalSize.height, canScroll[0], canScroll[1], canScroll[2], canScroll[3]);
}