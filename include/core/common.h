#ifndef _CORE_COMMON_H
#define _CORE_COMMON_H

typedef struct Dimensions
{
    int width;
    int height;
} Dimensions;

typedef struct Rectangle
{
    int x;
    int y;
    int width;
    int height;
} Rectangle;

/**
 * Whether or not the rectangle has a size of 0.
 */
bool isZeroSizeRectangle(const Rectangle& rect);

/**
 * @brief This function adds the x,y coordinates of rectangle b
 * to rectangle a and returns a new rectangle with these combined x,y coords and 
 * the width and height of rectangle a
 */
Rectangle addOffset(const Rectangle& a, const Rectangle& b);

/**
 * @brief Extract a Dimensions struct from a Rectangle that only contains the width and height
 */
Dimensions getDimensions(const Rectangle& r);

#endif