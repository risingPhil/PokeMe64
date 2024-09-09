#ifndef _CORE_COMMON_H
#define _CORE_COMMON_H

typedef struct Point
{
    int x;
    int y;
} Point;

typedef struct FloatPoint
{
    float x;
    float y;
} FloatPoint;

typedef Point Vector;
typedef FloatPoint FloatVector;

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
 * @brief This class wraps a raw char pointer.
 * This is to allow for automatic memory management without all of the bloat
 * of std::string. This is useful to pass it as part of a SceneContext and have
 * the string automatically released when the context gets deleted.
 */
class ManagedString
{
public:
    ManagedString(char* rawString);
    ~ManagedString();

    const char* get() const;

    void operator = (char* rawString);
protected:
private:
    char* rawString_;
};

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

bool isPointInsideRectangle(const Rectangle& a, const Point& p);

bool doRectanglesOverlap(const Rectangle& a, const Rectangle& b);

/**
 * @brief Extract a Dimensions struct from a Rectangle that only contains the width and height
 */
Dimensions getDimensions(const Rectangle& r);

#endif