#ifndef _RDPQGRAPHICS_H
#define _RDPQGRAPHICS_H

#include "core/common.h"

#include <cstdint>
#include <libdragon.h>

typedef struct TextRenderSettings
{
    uint8_t fontId;
    uint8_t fontStyleId;
    int16_t charSpacing; ///< Extra spacing between chars (in addition to glyph width and kerning)
    int16_t lineSpacing; ///< Extra spacing between lines (in addition to font height)
    rdpq_align_t halign;
    rdpq_valign_t valign;
} TextRenderSettings;

typedef struct SpriteRenderSettings SpriteRenderSettings;
typedef struct SurfaceRenderSettings SurfaceRenderSettings;

/**
 * @brief This class abstracts operations done with the RDPQ graphics API in libdragon
 * It probably wasn't necessary, but it makes me feel better :)
 */
class RDPQGraphics
{
public:
    RDPQGraphics();
    ~RDPQGraphics();

    void init();
    void destroy();

    void triggerDebugFrame();

    /**
     * @brief This function marks the start of a new frame.
     * In practice, it attaches the RDP to a new/different framebuffer and clears it
     */
    void beginFrame();

    /**
     * @brief This function marks the end of a frame
     * In practice, it detaches the RDP from a framebuffer and submits it for showing to the user.
     * (when it's done rendering)
     */
    void finishAndShowFrame();

    /**
     * @brief This function renders a filled rectangle with the specified color
     * at the specified absolute destination rectangle
     */
    void fillRectangle(const Rectangle& dstRect, color_t color);

    /**
     * This function can be used to draw text on screen with the specified fontId at the given destionation rectangle.
     * 
     * The thing is: there's no way to influence the font size: this is determined at compilation when you create the .font64 file in the Makefile.
     * So we need different font instances for different font sizes and all need to have been generated at compile time.
     */
    void drawText(const Rectangle& dstRect, const char* text, const TextRenderSettings& renderSettings);

    /**
     * @brief This function can be used to draw an image/sprite at the specified destination
     * with the specified SpriteRenderSettings. Please refer to SpriteRenderSettings for more info.
     */
    void drawSprite(const Rectangle& dstRect, sprite_t* sprite, const SpriteRenderSettings& renderSettings);

    /**
     * Same as drawSprite() but then for surface_t objects
     */
    void drawSurface(const Rectangle& dstRect, const surface_t* surface, const SurfaceRenderSettings& renderSettings);

    const Rectangle& getClippingRectangle() const;
    void setClippingRectangle(const Rectangle& clipRect);
    void resetClippingRectangle();
protected:
private:
    Rectangle clipRect_;
    bool initialized_;
    bool debugFrameTriggered_;
};

#endif