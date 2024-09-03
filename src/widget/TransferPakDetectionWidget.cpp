#include "widget/TransferPakDetectionWidget.h"
#include "transferpak/TransferPakManager.h"
#include "transferpak/TransferPakRomReader.h"
#include "transferpak/TransferPakSaveManager.h"
#include "tpak.h"

/**
 * @brief This function allows you to specify a 32 bit RGBA color by specifying separate color components
 * and converting it to a RGBA16 uint16_t value at compile time
 * 
 * @param r 8 bit red value
 * @param g 8 bit green value
 * @param b 8 bit blue value
 * @param a 8 bit alpha value 
 */
constexpr uint16_t colorToRGBA16(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return (((uint16_t)r >> 3) << 11) | (((uint16_t)g >> 3) << 6) | (((uint16_t)b >> 3) << 1) | (a >> 7);
}

static const Rectangle textBounds = {0, 100, 200, 20};
static const Rectangle cartridgeLabelBounds = {9, 26, 70, 62};

static const uint16_t paletteBlue[] = {0, colorToRGBA16(0x20, 0x30, 0x81, 0xFF), colorToRGBA16(0x15, 0x3B, 0xB0, 0xFF), 0, 0, 0, 0, 0};
static const uint16_t paletteRed[] = {0, colorToRGBA16(0xA7, 0x1F, 0x1B, 0xFF), colorToRGBA16(0xAA, 0x2A, 0x2A, 0xFF), 0, 0, 0, 0, 0};
static const uint16_t paletteYellow[] = {0, colorToRGBA16(0xEA, 0xA8, 0x2C, 0xFF), colorToRGBA16(0xF4, 0xB0, 0x2E, 0xFF), 0, 0, 0, 0, 0};
static const uint16_t paletteGold[] = {0, colorToRGBA16(0x8A, 0x86, 0x48, 0xFF), colorToRGBA16(0x88, 0x89, 0x4B, 0xFF), 0, 0, 0, 0, 0};
static const uint16_t paletteSilver[] = {0, colorToRGBA16(0x90, 0x8D, 0x85, 0xFF), colorToRGBA16(0xA2, 0x9E, 0x98, 0xFF), 0, 0, 0, 0, 0};
static const uint16_t paletteCrystal[] = {0, colorToRGBA16(0x55, 0x7A, 0x77, 0xFF), colorToRGBA16(0x72, 0x9E, 0xA4, 0xFF), 0, 0, 0, 0, 0};

#if 0
#include "gen2/Gen2GameReader.h"
static void doRandomShit(TransferPakManager& tpakManager)
{
    TransferPakRomReader romReader(tpakManager);
    TransferPakSaveManager saveManager(tpakManager);
    Gen2GameReader reader(romReader, saveManager, Gen2GameType::CRYSTAL);
    tpakManager.setRAMEnabled(true);
    debugf("first pokemon: %s\r\n", reader.getPokemonName(1));

    debugf("Trainer name: %s\r\n", reader.getTrainerName());
}
#endif

TransferPakDetectionWidget::TransferPakDetectionWidget(AnimationManager& animManager, TransferPakManager& pakManager)
    : style_({0})
    , animManager_(animManager)
    , tpakManager_(pakManager)
    , bounds_({0})
    , currentState_(TransferPakWidgetState::UNKNOWN)
    , previousInputState_({0})
    , gen1Type_(Gen1GameType::INVALID)
    , gen2Type_(Gen2GameType::INVALID)
    , stateChangedCallback_(nullptr)
    , stateChangedCallbackContext_(nullptr)
    , cartridgeIconSprite_(nullptr)
    , cartridgeLabelSprite_(nullptr)
    , cartridgeIconRenderSettings_()
    , cartridgeLabelRenderSettings_()
    , focused_(false)
    , visible_(true)
{
    cartridgeIconSprite_ = sprite_load("rom://cartridge-icon.sprite");
    cartridgeLabelSprite_ = sprite_load("rom://cartridge-label-unknown.sprite");
}

TransferPakDetectionWidget::~TransferPakDetectionWidget()
{
    sprite_free(cartridgeIconSprite_);
    cartridgeIconSprite_ = nullptr;

    if(cartridgeLabelSprite_)
    {
        sprite_free(cartridgeLabelSprite_);
        cartridgeLabelSprite_ = nullptr;
    }
}

bool TransferPakDetectionWidget::isFocused() const
{
    return focused_;
}

void TransferPakDetectionWidget::setFocused(bool focused)
{
    focused_ = focused;
}

bool TransferPakDetectionWidget::isVisible() const
{
    return visible_;
}

void TransferPakDetectionWidget::setVisible(bool visible)
{
    visible_ = visible;
}

Rectangle TransferPakDetectionWidget::getBounds() const
{
    return bounds_;
}

void TransferPakDetectionWidget::setBounds(const Rectangle& bounds)
{
    bounds_ = bounds;
}

Dimensions TransferPakDetectionWidget::getSize() const
{
    return Dimensions{ .width = bounds_.width, .height = bounds_.height };
}

bool TransferPakDetectionWidget::handleUserInput(const joypad_inputs_t& userInput)
{
    bool ret = false;
    if(previousInputState_.btn.a && !userInput.btn.a)
    {
        switch(currentState_)
        {
        case TransferPakWidgetState::UNKNOWN:
            switchState(currentState_, TransferPakWidgetState::DETECTING_PAK);
            ret = true;
            break;
        default:
            break;
        }
    }

    previousInputState_ = userInput;
    return ret;
}

void TransferPakDetectionWidget::render(RDPQGraphics& gfx, const Rectangle& parentBounds)
{
    //gfx.fillRectangle(bounds_, RGBA32(0xFF, 0, 0, 0xFF));
    const Rectangle absoluteIconBounds = {bounds_.x + ((bounds_.width - 88) / 2), bounds_.y, 88, 100};
    gfx.drawSprite(absoluteIconBounds, cartridgeIconSprite_, cartridgeIconRenderSettings_);

    if(cartridgeLabelSprite_)
    {
        const Rectangle absoluteLabelBounds = addOffset(cartridgeLabelBounds, absoluteIconBounds);
        gfx.drawSprite(absoluteLabelBounds, cartridgeLabelSprite_, cartridgeLabelRenderSettings_);
    }

    switch(currentState_)
    {
    case TransferPakWidgetState::UNKNOWN:
        renderUnknownState(gfx, parentBounds);
        break;
    case TransferPakWidgetState::NO_TRANSFER_PAK_FOUND:
    case TransferPakWidgetState::GB_HEADER_VALIDATION_FAILED:
    case TransferPakWidgetState::NO_GAME_FOUND:
        renderErrorState(gfx, parentBounds);
    default:
        break;
    }
}

TransferPakWidgetState TransferPakDetectionWidget::getState() const
{
    return currentState_;
}

void TransferPakDetectionWidget::retrieveGameType(Gen1GameType& outGen1Type, Gen2GameType& outGen2Type)
{
    outGen1Type = gen1Type_;
    outGen2Type = gen2Type_;
}

void TransferPakDetectionWidget::setStyle(const TransferPakDetectionWidgetStyle& style)
{
    style_ = style;
}

void TransferPakDetectionWidget::setStateChangedCallback(void (*callback)(void*, TransferPakWidgetState), void* context)
{
    stateChangedCallback_ = callback;
    stateChangedCallbackContext_ = context;
}

void TransferPakDetectionWidget::switchState(TransferPakWidgetState previousState, TransferPakWidgetState state)
{
    TransferPakWidgetState newState;
    bool ret;

    currentState_ = state;
    switch(state)
    {
    case TransferPakWidgetState::DETECTING_PAK:
        ret = selectTransferPak();
        newState = (ret) ? TransferPakWidgetState::VALIDATING_GB_HEADER : TransferPakWidgetState::NO_TRANSFER_PAK_FOUND;
        switchState(state, newState);
        return;
    case TransferPakWidgetState::VALIDATING_GB_HEADER:
        ret = validateGameboyHeader();
        newState = (ret) ? TransferPakWidgetState::DETECTING_GAME : TransferPakWidgetState::GB_HEADER_VALIDATION_FAILED;
        switchState(state, newState);
        return;
    case TransferPakWidgetState::DETECTING_GAME:
        ret = detectGameType();
        newState = (ret) ? TransferPakWidgetState::GAME_FOUND : TransferPakWidgetState::NO_GAME_FOUND;
        switchState(state, newState);
        return;
    case TransferPakWidgetState::GAME_FOUND:
        updateCartridgeIcon();
//      doRandomShit(tpakManager_);
        break;
    default:
        break;
    }

    // now notify the callback (if any) that the state has changed
    if(stateChangedCallback_)
    {
        stateChangedCallback_(stateChangedCallbackContext_, state);
    }
}

void TransferPakDetectionWidget::renderUnknownState(RDPQGraphics& gfx, const Rectangle& parentBounds)
{
    const Rectangle absoluteTextBounds = addOffset(textBounds, bounds_);
    gfx.drawText(absoluteTextBounds, "Press A to start", style_.textSettings);
}

void TransferPakDetectionWidget::renderErrorState(RDPQGraphics& gfx, const Rectangle& parentBounds)
{
    const Rectangle absoluteTextBounds = addOffset(textBounds, bounds_);
    const char* errorText;

    switch(currentState_)
    {
    case TransferPakWidgetState::NO_TRANSFER_PAK_FOUND:
        errorText = "ERROR: No Transfer Pak found!";
        break;
    case TransferPakWidgetState::GB_HEADER_VALIDATION_FAILED:
        errorText = "ERROR: Gameboy Header validation failed!";
        break;
    case TransferPakWidgetState::NO_GAME_FOUND:
        // TODO: technically this is not correct
        // We just didn't find a Pkmn game.
        errorText = "ERROR: No game found!";
        break;
    default:
        errorText = "ERROR: this should never happen!";
        break;
    }

    gfx.drawText(absoluteTextBounds, errorText, style_.textSettings);
}

bool TransferPakDetectionWidget::selectTransferPak()
{
    joypad_poll();
    for(uint8_t i=0; i < 4; ++i)
    {
        tpakManager_.setPort((joypad_port_t)i);
        if(tpakManager_.hasTransferPak())
        {
            debugf("[Application]: Transfer pak found at controller %hu\r\n", i);
            // power the transfer pak off in case it was powered before
            tpakManager_.setPower(false);
            return true;
        }
    }
    debugf("[Application]: ERROR: no transfer pak found!\r\n");
    return false;
}

bool TransferPakDetectionWidget::validateGameboyHeader()
{
    gameboy_cartridge_header cartridgeHeader;
    if(!tpakManager_.setPower(true))
    {
        return false;
    }


    if(!tpakManager_.readCartridgeHeader(cartridgeHeader))
    {
        return false;
    }

    if(!tpak_check_header(&cartridgeHeader))
    {
        debugf("[TransferPakDetectionWidget]: ERROR: tpak_check_header returned false!\r\n");
        return false;
    }

    return true;
}

bool TransferPakDetectionWidget::detectGameType()
{
    GameboyCartridgeHeader cartridgeHeader;
    TransferPakRomReader romReader(tpakManager_);

    readGameboyCartridgeHeader(romReader, cartridgeHeader);

    gen1Type_ = gen1_determineGameType(cartridgeHeader);
    gen2Type_ = gen2_determineGameType(cartridgeHeader);

    return (gen1Type_ != Gen1GameType::INVALID || gen2Type_ != Gen2GameType::INVALID);
}

void TransferPakDetectionWidget::updateCartridgeIcon()
{
    const char* labelSpritePath = nullptr;
    if(gen1Type_ != Gen1GameType::INVALID)
    {
        switch(gen1Type_)
        {
        case Gen1GameType::BLUE:
            cartridgeIconRenderSettings_.customPalette.numColors = sizeof(paletteBlue) / sizeof(paletteBlue[0]);
            cartridgeIconRenderSettings_.customPalette.colorsRGBA16 = paletteBlue;
            labelSpritePath = "rom://cartridge-label-blue.sprite";
            break;
        case Gen1GameType::RED:
            cartridgeIconRenderSettings_.customPalette.numColors = sizeof(paletteRed) / sizeof(paletteRed[0]);
            cartridgeIconRenderSettings_.customPalette.colorsRGBA16 = paletteRed;
            labelSpritePath = "rom://cartridge-label-red.sprite";
            break;
        case Gen1GameType::YELLOW:
            cartridgeIconRenderSettings_.customPalette.numColors = sizeof(paletteYellow) / sizeof(paletteYellow[0]);
            cartridgeIconRenderSettings_.customPalette.colorsRGBA16 = paletteYellow;
            labelSpritePath = "rom://cartridge-label-yellow.sprite";
            break;
        default:
            break;
        }
    }
    else if(gen2Type_ != Gen2GameType::INVALID)
    {
        switch(gen2Type_)
        {
        case Gen2GameType::GOLD:
            cartridgeIconRenderSettings_.customPalette.numColors = sizeof(paletteGold) / sizeof(paletteGold[0]);
            cartridgeIconRenderSettings_.customPalette.colorsRGBA16 = paletteGold;
            labelSpritePath = "rom://cartridge-label-gold.sprite";
            break;
        case Gen2GameType::SILVER:
            cartridgeIconRenderSettings_.customPalette.numColors = sizeof(paletteSilver) / sizeof(paletteSilver[0]);
            cartridgeIconRenderSettings_.customPalette.colorsRGBA16 = paletteSilver;
            labelSpritePath = "rom://cartridge-label-silver.sprite";
            break;
        case Gen2GameType::CRYSTAL:
            cartridgeIconRenderSettings_.customPalette.numColors = sizeof(paletteCrystal) / sizeof(paletteCrystal[0]);
            cartridgeIconRenderSettings_.customPalette.colorsRGBA16 = paletteCrystal;
            labelSpritePath = "rom://cartridge-label-crystal.sprite";
            break;
        default:
            break;
        }
    }

    if(labelSpritePath)
    {
        if(cartridgeLabelSprite_)
        {
            sprite_free(cartridgeLabelSprite_);
        }
        cartridgeLabelSprite_ = sprite_load(labelSpritePath);
    }
}