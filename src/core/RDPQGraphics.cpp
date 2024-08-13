#include "core/RDPQGraphics.h"
#include "core/Sprite.h"

static void render_sprite_normal(const Rectangle &dstRect, sprite_t *sprite, const SpriteRenderSettings &renderSettings)
{
    if (!isZeroSizeRectangle(renderSettings.srcRect))
    {
        const rdpq_blitparms_t blitParams = {
            .s0 = renderSettings.srcRect.x,
            .t0 = renderSettings.srcRect.y,
            .width = renderSettings.srcRect.width,
            .height = renderSettings.srcRect.height,
            .scale_x = static_cast<float>(dstRect.width) / renderSettings.srcRect.width,
            .scale_y = static_cast<float>(dstRect.height) / renderSettings.srcRect.height,
            .theta = renderSettings.rotationAngle
        };

        rdpq_sprite_blit(sprite, dstRect.x, dstRect.y, &blitParams);
    }
    else
    {
        const rdpq_blitparms_t blitParams = {
            .scale_x = static_cast<float>(dstRect.width) / sprite->width,
            .scale_y = static_cast<float>(dstRect.height) / sprite->height,
            .theta = renderSettings.rotationAngle
        };

        rdpq_sprite_blit(sprite, dstRect.x, dstRect.y, &blitParams);
    }
}

static void render_sprite_ninegrid(const Rectangle &dstRect, sprite_t *sprite, const SpriteRenderSettings &renderSettings)
{
    // left top corner
    Rectangle curDest = {
        .x = dstRect.x,
        .y = dstRect.y,
        .width = renderSettings.srcRect.x, 
        .height = renderSettings.srcRect.y
    };
    Rectangle curSrc = {
        .x = 0,
        .y = 0,
        .width = renderSettings.srcRect.x,
        .height = renderSettings.srcRect.y
    };
    render_sprite_normal(curDest, sprite, {.renderMode = SpriteRenderMode::NORMAL, .srcRect = curSrc});

    // top edge
    curDest = {
        .x = dstRect.x + renderSettings.srcRect.x,
        .y = dstRect.y,
        .width = dstRect.width - renderSettings.srcRect.width - renderSettings.srcRect.x,
        .height = renderSettings.srcRect.y 
    };
    curSrc = {
        .x = renderSettings.srcRect.x,
        .y = 0,
        .width = sprite->width - renderSettings.srcRect.width - renderSettings.srcRect.x,
        .height = renderSettings.srcRect.y
    };
    render_sprite_normal(curDest, sprite, {.renderMode = SpriteRenderMode::NORMAL, .srcRect = curSrc});

    // right top corner
    curDest = {
        .x = dstRect.x + dstRect.width - renderSettings.srcRect.width,
        .y = dstRect.y,
        .width = renderSettings.srcRect.width,
        .height = renderSettings.srcRect.y
    };
    curSrc = {
        .x = sprite->width - renderSettings.srcRect.width,
        .y = 0,
        .width = renderSettings.srcRect.width,
        .height = renderSettings.srcRect.y
    };
    render_sprite_normal(curDest, sprite, {.renderMode = SpriteRenderMode::NORMAL, .srcRect = curSrc});

    // right edge
    curDest = {
        .x = dstRect.x + dstRect.width - renderSettings.srcRect.width,
        .y = dstRect.y + renderSettings.srcRect.y,
        .width = renderSettings.srcRect.width,
        .height = dstRect.height - renderSettings.srcRect.height - renderSettings.srcRect.y
    };
    curSrc = {
        .x = sprite->width - renderSettings.srcRect.width,
        .y = renderSettings.srcRect.y,
        .width = renderSettings.srcRect.width,
        .height = sprite->height - renderSettings.srcRect.height - renderSettings.srcRect.y
    };
    render_sprite_normal(curDest, sprite, {.renderMode = SpriteRenderMode::NORMAL, .srcRect = curSrc});

    // bottom right corner
    curDest = {
        .x = dstRect.x + dstRect.width - renderSettings.srcRect.width,
        .y = dstRect.y + dstRect.height - renderSettings.srcRect.height,
        .width = renderSettings.srcRect.width,
        .height = renderSettings.srcRect.height
    };
    curSrc = {
        .x = sprite->width - renderSettings.srcRect.width,
        .y = sprite->height - renderSettings.srcRect.height,
        .width = renderSettings.srcRect.width,
        .height = renderSettings.srcRect.height
    };
    render_sprite_normal(curDest, sprite, {.renderMode = SpriteRenderMode::NORMAL, .srcRect = curSrc});

    // bottom edge
    curDest = {
        .x = dstRect.x + renderSettings.srcRect.x,
        .y = dstRect.y + dstRect.height - renderSettings.srcRect.height,
        .width = dstRect.width - renderSettings.srcRect.width - renderSettings.srcRect.x,
        .height = renderSettings.srcRect.height
    };
    curSrc = {
        .x = renderSettings.srcRect.x,
        .y = sprite->height - renderSettings.srcRect.height,
        .width = sprite->width - renderSettings.srcRect.width - renderSettings.srcRect.x,
        .height = renderSettings.srcRect.height
    };
    render_sprite_normal(curDest, sprite, {.renderMode = SpriteRenderMode::NORMAL, .srcRect = curSrc});

    // bottom left corner
    curDest = {
        .x = dstRect.x,
        .y = dstRect.y + dstRect.height - renderSettings.srcRect.height,
        .width = renderSettings.srcRect.x,
        .height = renderSettings.srcRect.height
    };
    curSrc = {
        .x = 0,
        .y = sprite->height - renderSettings.srcRect.height,
        .width = renderSettings.srcRect.x,
        .height = renderSettings.srcRect.height
    };
    render_sprite_normal(curDest, sprite, {.renderMode = SpriteRenderMode::NORMAL, .srcRect = curSrc});

    // left edge
    curDest = {
        .x = dstRect.x,
        .y = dstRect.y + renderSettings.srcRect.y,
        .width = renderSettings.srcRect.x,
        .height = dstRect.height - renderSettings.srcRect.height - renderSettings.srcRect.y
    };
    curSrc = {
        .x = 0,
        .y = renderSettings.srcRect.y,
        .width = renderSettings.srcRect.x,
        .height = sprite->height - renderSettings.srcRect.height - renderSettings.srcRect.y
    };
    render_sprite_normal(curDest, sprite, {.renderMode = SpriteRenderMode::NORMAL, .srcRect = curSrc});

    // inner container
    curDest = {
        .x = dstRect.x + renderSettings.srcRect.x,
        .y = dstRect.y + renderSettings.srcRect.y,
        .width = dstRect.width - renderSettings.srcRect.width - renderSettings.srcRect.x,
        .height = dstRect.height - renderSettings.srcRect.height - renderSettings.srcRect.y
    };
    curSrc = {
        .x = renderSettings.srcRect.x,
        .y = renderSettings.srcRect.y,
        .width = sprite->width - renderSettings.srcRect.width - renderSettings.srcRect.x,
        .height = sprite->height - renderSettings.srcRect.height - renderSettings.srcRect.y
    };
    render_sprite_normal(curDest, sprite, {.renderMode = SpriteRenderMode::NORMAL, .srcRect = curSrc});
}

// same as render_sprite_normal, but for surface_t
static void render_surface(const Rectangle &dstRect, const surface_t *surface, const SurfaceRenderSettings &renderSettings)
{
    if (!isZeroSizeRectangle(renderSettings.srcRect))
    {
        const rdpq_blitparms_t blitParams = {
            .s0 = renderSettings.srcRect.x,
            .t0 = renderSettings.srcRect.y,
            .width = renderSettings.srcRect.width,
            .height = renderSettings.srcRect.height,
            .scale_x = static_cast<float>(dstRect.width) / renderSettings.srcRect.width,
            .scale_y = static_cast<float>(dstRect.height) / renderSettings.srcRect.height,
            .theta = renderSettings.rotationAngle
        };

        rdpq_tex_blit(surface, dstRect.x, dstRect.y, &blitParams);
    }
    else
    {
        const rdpq_blitparms_t blitParams = {
            .scale_x = static_cast<float>(dstRect.width) / surface->width,
            .scale_y = static_cast<float>(dstRect.height) / surface->height,
            .theta = renderSettings.rotationAngle
        };

        rdpq_tex_blit(surface, dstRect.x, dstRect.y, &blitParams);
    }
}

RDPQGraphics::RDPQGraphics()
    : clipRect_({0})
    , initialized_(false)
    , debugFrameTriggered_(false)
{
}

RDPQGraphics::~RDPQGraphics()
{
}

void RDPQGraphics::init()
{
    rdpq_init();
    rdpq_debug_start();
    initialized_ = true;
}

void RDPQGraphics::destroy()
{
    rdpq_debug_stop();
    rdpq_close();
    initialized_ = false;
}

void RDPQGraphics::triggerDebugFrame()
{
    rdpq_debug_log(true);
    debugFrameTriggered_ = true;
}

void RDPQGraphics::beginFrame()
{
    // Attach and clear the screen
    surface_t *disp = display_get();
    rdpq_attach_clear(disp, NULL);
}

void RDPQGraphics::finishAndShowFrame()
{
    rdpq_detach_show();
    if(debugFrameTriggered_)
    {
        rdpq_debug_log(false);
        debugFrameTriggered_ = false;
    }
}

void RDPQGraphics::fillRectangle(const Rectangle &dstRect, color_t color)
{
    rdpq_mode_push();
    rdpq_set_mode_fill(color);
    rdpq_fill_rectangle(dstRect.x, dstRect.y, dstRect.x + dstRect.width, dstRect.y + dstRect.height);
    rdpq_mode_pop();
}

void RDPQGraphics::drawText(const Rectangle &dstRect, const char *text, const TextRenderSettings& renderSettings)
{
    // disable_interrupts();
    // uint32_t t0 = get_ticks();
    rdpq_textparms_t textParams = {
        // .line_spacing = -3,
        .style_id = renderSettings.fontStyleId,
        .width = static_cast<int16_t>(dstRect.width),
        .height = static_cast<int16_t>(dstRect.height),
        .align = ALIGN_LEFT,
        .valign = VALIGN_TOP,
        .char_spacing = renderSettings.charSpacing,
        .line_spacing = renderSettings.lineSpacing,
        .wrap = WRAP_WORD,
    };

    rdpq_text_print(&textParams, renderSettings.fontId, dstRect.x, dstRect.y, text);
    
    // TODO: this is a temporary workaround for a bug I reported to the libdragon team on discord on 05/07/2024
    // https://discord.com/channels/205520502922543113/974342113850445874
    // Don't forget to remove it after it has been fixed.
    rdpq_sync_tile();

    // rdpq_paragraph_render(partext, (320-box_width)/2, (240-box_height)/2);
    // uint32_t t1 = get_ticks();
    // enable_interrupts();
}

void RDPQGraphics::drawSprite(const Rectangle &dstRect, sprite_t *sprite, const SpriteRenderSettings &renderSettings)
{
    rdpq_mode_begin();
    rdpq_set_mode_standard();
    rdpq_mode_alphacompare(1); // colorkey (draw pixel with alpha >= 1)
    switch(sprite_get_format(sprite))
    {
        case FMT_RGBA32:
        case FMT_IA8:
        case FMT_IA16:
            rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
            break;
        default:
            break;
    }
    rdpq_mode_filter(FILTER_BILINEAR);
    rdpq_mode_end();

    switch (renderSettings.renderMode)
    {
    case SpriteRenderMode::NORMAL:
        render_sprite_normal(dstRect, sprite, renderSettings);
        break;
    case SpriteRenderMode::NINESLICE:
        render_sprite_ninegrid(dstRect, sprite, renderSettings);
        break;
    default:
        break;
    }
}

void RDPQGraphics::drawSurface(const Rectangle& dstRect, const surface_t* surface, const SurfaceRenderSettings& renderSettings)
{
    rdpq_mode_begin();
    rdpq_set_mode_standard();
    rdpq_mode_alphacompare(1); // colorkey (draw pixel with alpha >= 1)
    switch(surface_get_format(surface))
    {
        case FMT_RGBA32:
        case FMT_IA8:
        case FMT_IA16:
            rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
            break;
        default:
            break;
    }
    rdpq_mode_filter(FILTER_BILINEAR);
    rdpq_mode_end();

    render_surface(dstRect, surface, renderSettings);
}

const Rectangle& RDPQGraphics::getClippingRectangle() const
{
    return clipRect_;
}

void RDPQGraphics::setClippingRectangle(const Rectangle& clipRect)
{
    clipRect_ = clipRect;
    rdpq_set_scissor(clipRect.x, clipRect.y, clipRect.x + clipRect.width, clipRect.y + clipRect.height);
}

void RDPQGraphics::resetClippingRectangle()
{
    setClippingRectangle({ .x = 0, .y = 0, .width = static_cast<int>(display_get_width()), .height = static_cast<int>(display_get_height()) });
}