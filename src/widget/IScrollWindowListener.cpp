#include "widget/IScrollWindowListener.h"
#include <libdragon.h>

IScrollWindowListener::~IScrollWindowListener()
{
}

TestScrollWindowListener::~TestScrollWindowListener()
{
}

void TestScrollWindowListener::onScrollWindowChanged(const ScrollWindowUpdate &update)
{
    debugf("[TestScrollWindowListener]:%s:%p: scrollWindowRectangle: [%d, %d, %d, %d], totalSize: [%d, %d]\r\n", __FUNCTION__, this, update.scrollWindowRectangle.x, update.scrollWindowRectangle.y, update.scrollWindowRectangle.width, update.scrollWindowRectangle.height, update.totalSize.width, update.totalSize.height);
}