#include "widget/PokemonPartyIconWidget.h"
#include "core/RDPQGraphics.h"
#include "gen1/Gen1GameReader.h"
#include "gen2/Gen2GameReader.h"
#include "SaveManager.h"
#include "SpriteRenderer.h"

#define DEFAULT_FPS_WHEN_FOCUSED 4
#define DEFAULT_FPS_WHEN_NOT_FOCUSED 2

static uint32_t calculateFrameSwitchTimeout(uint8_t fps)
{
    return TICKS_FROM_MS(1000u / fps);
}

PokemonPartyIconFactory::PokemonPartyIconFactory(IRomReader& romReader)
    : iconMap_()
    , romReader_(romReader)
{
    memset(iconMap_, 0, sizeof(IconMapEntry) * Gen2PokemonIconType::GEN2_ICONTYPE_MAX);
}

PokemonPartyIconFactory::~PokemonPartyIconFactory()
{
    for(uint8_t i = 0; i < (uint8_t)Gen2PokemonIconType::GEN2_ICONTYPE_MAX; ++i)
    {
        if(iconMap_[i].frame1.buffer)
        {
            surface_free(&iconMap_[i].frame1);
        }
        if(iconMap_[i].frame2.buffer)
        {
            surface_free(&iconMap_[i].frame2);
        }
    }
}

surface_t PokemonPartyIconFactory::getIcon(uint8_t generation, uint8_t specificGenType, uint8_t localization, uint8_t iconType, bool firstFrame)
{
    BufferBasedSaveManager dummy(nullptr, 0);
    SpriteRenderer spriteRenderer;
    surface_t result = (firstFrame) ? iconMap_[iconType].frame1 : iconMap_[iconType].frame2;
    const uint8_t* outputBuffer = nullptr;
    uint8_t iconWidthInPixels;
    uint8_t iconHeightInPixels;
    // For RGBA16, we have 2 bytes per color
    const uint8_t bytesPerColor = 2;

    if(result.buffer)
    {
        return result;
    }

    if(generation == 1)
    {
        Gen1GameReader gameReader(romReader_, dummy, static_cast<Gen1GameType>(specificGenType), static_cast<Gen1LocalizationLanguage>(localization));
        iconWidthInPixels = GEN1_ICON_WIDTH_IN_TILES * 8;
        iconHeightInPixels = GEN1_ICON_HEIGHT_IN_TILES * 8;

        const uint8_t* spriteBuffer = gameReader.decodePokemonIcon((Gen1PokemonIconType)iconType, firstFrame);
        if(!spriteBuffer)
        {
            return result;
        }
        spriteRenderer.draw(spriteBuffer, SpriteRenderer::OutputFormat::RGBA16, monochromeGBColorPalette, GEN1_ICON_WIDTH_IN_TILES, GEN1_ICON_HEIGHT_IN_TILES);
        outputBuffer = spriteRenderer.removeWhiteBackground(GEN1_ICON_WIDTH_IN_TILES, GEN1_ICON_HEIGHT_IN_TILES);
    }
    else if(generation == 2)
    {
        Gen2GameReader gameReader(romReader_, dummy, static_cast<Gen2GameType>(specificGenType), static_cast<Gen2LocalizationLanguage>(localization));
        iconWidthInPixels = GEN2_ICON_WIDTH_IN_TILES * 8;
        iconHeightInPixels = GEN2_ICON_HEIGHT_IN_TILES * 8;

        const uint8_t* spriteBuffer = gameReader.decodePokemonIcon((Gen2PokemonIconType)iconType, firstFrame);
        if(!spriteBuffer)
        {
            return result;
        }
        spriteRenderer.draw(spriteBuffer, SpriteRenderer::OutputFormat::RGBA16, gen2_iconColorPalette, GEN2_ICON_WIDTH_IN_TILES, GEN2_ICON_HEIGHT_IN_TILES);
        outputBuffer = spriteRenderer.removeWhiteBackground(GEN2_ICON_WIDTH_IN_TILES, GEN2_ICON_HEIGHT_IN_TILES);
    }
    else
    {
        debugf("Invalid generation %hu!\n", generation);
        return result;
    }

    // due to alignment constraints, we should use the surface_alloc function to ensure that it is done correctly
    // and respect its resulting stride field
    result = surface_alloc(FMT_RGBA16, iconWidthInPixels, iconHeightInPixels);
    
    //now copy the sprite to the surface, line by line
    const uint32_t actualStride = iconWidthInPixels * bytesPerColor;
    for(uint8_t i=0; i < iconHeightInPixels; ++i)
    {
        const uint8_t* src = outputBuffer + (i * actualStride);
        uint8_t* dst = ((uint8_t*)(result.buffer)) + (i * result.stride);
        // copy the source buffer to the surface line by line, but advancing with the surface stride on every new row
        memcpy(dst, src, actualStride);
    }

    // now we have a valid surface_t. Let's store it in our map
    if(firstFrame)
    {
        iconMap_[iconType].frame1 = result;
    }
    else
    {
        iconMap_[iconType].frame2 = result;
    }

    return result;
}

PokemonPartyIconWidget::PokemonPartyIconWidget()
    : style_({0})
    , data_({0})
    , iconFrame1_({0})
    , iconFrame2_({0})
    , bounds_({0})
    , frameSwitchTimeoutInTicks_(0)
    , nextFrameSwitchTime_(0)
    , focused_(false)
    , visible_(true)
    , showFirstIconFrame_(true)
{
}

PokemonPartyIconWidget::~PokemonPartyIconWidget()
{
}

void PokemonPartyIconWidget::setStyle(const PokemonPartyIconWidgetStyle& style)
{
    style_ = style;

    reset();
}

void PokemonPartyIconWidget::setData(const PokemonPartyIconWidgetData& data)
{
    data_ = data;

    if(!data.iconFactory)
    {
        debugf("ERROR: No PokemonPartyIconFactory instance was provided!\n");
        return;
    }

    iconFrame1_ = data.iconFactory->getIcon(data_.generation, data_.specificGenVersion, data_.localization, data_.iconType, true);
    iconFrame2_ = data.iconFactory->getIcon(data_.generation, data_.specificGenVersion, data_.localization, data_.iconType, false);

    reset();
}

bool PokemonPartyIconWidget::isFocused() const
{
    return focused_;
}

void PokemonPartyIconWidget::setFocused(bool focused)
{
    focused_ = focused;

    reset();
}

bool PokemonPartyIconWidget::isVisible() const
{
    return visible_;
}

void PokemonPartyIconWidget::setVisible(bool visible)
{
    visible_ = visible;

    reset();
}

Rectangle PokemonPartyIconWidget::getBounds() const
{
    return bounds_;
}

void PokemonPartyIconWidget::setBounds(const Rectangle& bounds)
{
    bounds_ = bounds;
}

Dimensions PokemonPartyIconWidget::getSize() const
{
    return Dimensions{bounds_.width, bounds_.height};
}

bool PokemonPartyIconWidget::handleUserInput(const joypad_inputs_t&)
{
    return false;
}

void PokemonPartyIconWidget::render(RDPQGraphics& gfx, const Rectangle& parentBounds)
{
    const SurfaceRenderSettings renderSettings = { 0 };
    const Rectangle absoluteBounds = addOffset(bounds_, parentBounds);
    const Rectangle spriteBounds = addOffset(style_.icon.bounds, absoluteBounds);

    const uint64_t currentTicks = get_ticks();
    if(currentTicks >= nextFrameSwitchTime_)
    {
        showFirstIconFrame_ = !showFirstIconFrame_;
        nextFrameSwitchTime_ += frameSwitchTimeoutInTicks_;
    }
    
    if(style_.background.sprite)
    {
        gfx.drawSprite(absoluteBounds, style_.background.sprite, style_.background.spriteSettings);
    }

    if(showFirstIconFrame_)
    {
        if(iconFrame1_.buffer)
        {
            gfx.drawSurface(spriteBounds, &iconFrame1_, renderSettings);
        }
    }
    else
    {
        if(iconFrame2_.buffer)
        {
            gfx.drawSurface(spriteBounds, &iconFrame2_, renderSettings);
        }
        else if(iconFrame1_.buffer)
        {
            const Rectangle modifiedSpriteBounds = { .x = spriteBounds.x, .y = spriteBounds.y + style_.icon.yOffsetWhenTheresNoFrame2, .width = spriteBounds.width, .height = spriteBounds.height };
            gfx.drawSurface(modifiedSpriteBounds, &iconFrame1_, renderSettings);
        }
    }
}

void PokemonPartyIconWidget::reset()
{
    const uint8_t fps = (focused_) ? style_.fpsWhenFocused : style_.fpsWhenNotFocused;
    if(fps)
    {
        frameSwitchTimeoutInTicks_ = calculateFrameSwitchTimeout(fps);
        nextFrameSwitchTime_ = get_ticks() + frameSwitchTimeoutInTicks_;
    }
    else
    {
        nextFrameSwitchTime_ = 0xFFFFFFFFFFFFFFFF;
    }

    showFirstIconFrame_ = true;
}