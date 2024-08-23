#ifndef _SPRITE_H
#define _SPRITE_H

#include "core/common.h"

typedef struct sprite_s sprite_t;


/**
 * The render mode of the sprite. We have NORMAL and NINESLICE
 * NINESLICE is used to render and stretch a 9sliced image similar to 9slice images
 * are/were used on the web. (for things like rounded corners or otherwise custom boxes)
 */
enum class SpriteRenderMode
{
    NORMAL = 0,
    NINESLICE
};

typedef struct SpriteRenderSettings
{
    /*
     * Indicates the render mode of the sprite
     * 
     * If you use NINESLICE, you MUST specify a 9 slice rectangle in the srcRect field.
     * Please refer to the comments there for more info.
     */
    SpriteRenderMode renderMode;

    /**
     * Either a source region within the sprite that needs to be rendered if the SpriteRenderMode is set to NORMAL
     * or a 9slice rectangle if the SpriteRenderMode is set to NINESLICE
     * 
     * A 9slice rectangle is special because it is usually used to specify a box-like image with
     * special corners/edges using an extremely small image.
     * 
     * One typical use case for this is when you want to draw rounded corners.
     * 
     * To specify a 9slice image, the x, y, width and height properties of srcRect have a different meaning:
     * x -> the width of each corner on the left size of the image
     * y -> the height of each corner on the left size of the image
     * width -> the width of each corner on the right size of the image
     * height -> the height of each corner on the right size of the image
     * 
     * Our RDPQGraphics class will use these values to stretch a 9slice image according to the
     * destination rectangle specified in RDPQGraphics::drawSprite().
     * The corners will never get stretched, but the edges and middle portion will
     * 
     * Note: because of the properties of a 9slice image, the image doesn't really need to be any bigger
     * than the widths and heights of all corners combined + 1 pixel in each direction.
     * 
     * For example: if my corners are all 6x6 pixels, then it suffices to create an image that is 13x13 pixels
     * to render the box element.
     */
    Rectangle srcRect;
    
    /**
     * @brief Rotation angle in radians
     */
    float rotationAngle;
    struct {
        /**
         * WARNING: needs to be a multiple of 8
         */
        uint8_t numColors;
        /**
         * @brief If set, this RGBA16 array will replace the original color palette for the specified sprite
         */
        const uint16_t* colorsRGBA16;
    } customPalette;
} SpriteRenderSettings;

typedef struct SurfaceRenderSettings
{
    /**
     * Source region within the sprite that needs to be rendered (partial rendering)
     */
    Rectangle srcRect;

    /**
     * @brief Rotation angle in radians
     */
    float rotationAngle;
} SurfaceRenderSettings;

#endif