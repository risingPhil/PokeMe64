#include "widget/TransferPakDetectionWidget.h"
#include "transferpak/TransferPakManager.h"
#include "transferpak/TransferPakRomReader.h"
#include "transferpak/TransferPakSaveManager.h"

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
    , textBounds_({.x = 0, .y = 0, .width = 200, .height = 20})
    , currentState_(TransferPakWidgetState::UNKNOWN)
    , previousInputState_({0})
    , gen1Type_(Gen1GameType::INVALID)
    , gen2Type_(Gen2GameType::INVALID)
    , stateChangedCallback_(nullptr)
    , stateChangedCallbackContext_(nullptr)
    , focused_(false)
    , visible_(true)
{
}

TransferPakDetectionWidget::~TransferPakDetectionWidget()
{
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
    const Rectangle absoluteTextBounds = addOffset(textBounds_, bounds_);
    gfx.drawText(absoluteTextBounds, "Press A to check the transfer pak...", style_.textSettings);
}

void TransferPakDetectionWidget::renderErrorState(RDPQGraphics& gfx, const Rectangle& parentBounds)
{
    const Rectangle absoluteTextBounds = addOffset(textBounds_, bounds_);
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
    if(!tpakManager_.setPower(true))
    {
        return false;
    }
    return tpakManager_.validateGbHeader();
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