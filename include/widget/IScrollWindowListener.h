#ifndef ISCROLLWINDOWLISTENER_H
#define ISCROLLWINDOWLISTENER_H

#include "core/common.h"
#include "core/DragonUtils.h"

/**
 * This struct contains the various things to know about the scroll window
 * when it has been changed.
 */
typedef struct ScrollWindowUpdate
{
    /**
     * The coords and size of the scroll window relative to the widget
     * that implements the scrolling
     */
    Rectangle scrollWindowRectangle;
    /**
     * The total size of the panel containing all of the widgets in the scrolling widget.
     * this includes the parts that aren't visible (because they're not in the scroll window)
     */
    Dimensions totalSize;
} ScrollWindowUpdate;

bool canScrollTo(const ScrollWindowUpdate& info, UINavigationDirection direction);

/**
 * This class allows you to react to changes to the scroll window of a widget.
 * This is useful for showing UI indications like scrollbars or "more" arrows
 */
class IScrollWindowListener
{
public:
    virtual ~IScrollWindowListener();

    virtual void onScrollWindowChanged(const ScrollWindowUpdate& update) = 0;
protected:
private:
};

/**
 * Test implementation that will just print the scroll window update
 */
class TestScrollWindowListener : public IScrollWindowListener
{
public:
    virtual ~TestScrollWindowListener();

    void onScrollWindowChanged(const ScrollWindowUpdate& update) override;
protected:
private:
};

#endif