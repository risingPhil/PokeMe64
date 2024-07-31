#include "core/common.h"

bool isZeroSizeRectangle(const Rectangle &rect)
{
    return (!rect.width || !rect.height);
}

Rectangle addOffset(const Rectangle &a, const Rectangle &b)
{
    return Rectangle{.x = a.x + b.x, .y = a.y + b.y, .width = a.width, .height = a.height};
}

bool isPointInsideRectangle(const Rectangle& a, const Point& p)
{
    const bool isInsideHorizontally = (p.x >= a.x) && (p.x < (a.x + a.width));
    const bool isInsideVertically = (p.y >= a.y) && (p.y < (a.y + a.width));
    return (isInsideHorizontally && isInsideVertically);
}

bool doRectanglesOverlap(const Rectangle& a, const Rectangle& b)
{
    if (a.x < (b.x + b.width) && (a.x + a.width) > b.x &&
        a.y + a.height > b.y && a.y < (b.y + b.height) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

Dimensions getDimensions(const Rectangle &r)
{
    return Dimensions{.width = r.width, .height = r.height};
}