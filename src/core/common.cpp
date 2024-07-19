#include "core/common.h"

bool isZeroSizeRectangle(const Rectangle &rect)
{
    return (!rect.width || !rect.height);
}

Rectangle addOffset(const Rectangle &a, const Rectangle &b)
{
    return Rectangle{.x = a.x + b.x, .y = a.y + b.y, .width = a.width, .height = a.height};
}

Dimensions getDimensions(const Rectangle &r)
{
    return Dimensions{.width = r.width, .height = r.height};
}