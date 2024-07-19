#ifndef _TRANSFERPAKDETECTIONWIDGET_H
#define _TRANSFERPAKDETECTIONWIDGET_H

#include "widget/IWidget.h"
#include "core/Sprite.h"
#include "core/RDPQGraphics.h"
#include "gen1/Gen1Common.h"
#include "gen2/Gen2Common.h"

class AnimationManager;
class TransferPakManager;

enum class TransferPakWidgetState
{
    UNKNOWN,
    DETECTING_PAK,
    VALIDATING_GB_HEADER,
    DETECTING_GAME,
    GB_HEADER_VALIDATION_FAILED,
    NO_TRANSFER_PAK_FOUND,
    NO_GAME_FOUND,
    GAME_FOUND
};

typedef struct TransferPakDetectionWidgetStyle
{
    TextRenderSettings textSettings;
} TransferPakDetectionWidgetStyle;

/**
 * @brief This widget is used to handle the transfer pak detection process.
 * One of the major reasons for this widget is because libdragon's joypad polling is done in the background during interrupts
 * and therefore may not be ready when we're initializing the scene. Best thing to do is to wait for key input
 * 
 * So I might as well turn it into something visible.
 */
class TransferPakDetectionWidget : public IWidget
{
public:
    TransferPakDetectionWidget(AnimationManager& animManager, TransferPakManager& pakManager);
    virtual ~TransferPakDetectionWidget();

    bool isFocused() const override;
    
    /**
     * @brief Sets whether the widget is currently focused
     * 
     */
    void setFocused(bool isFocused) override;

    /**
     * @brief Returns whether the widget is currently visible
     */
    bool isVisible() const override;
    
    /**
     * @brief Changes the visibility of the widget
     */
    void setVisible(bool visible) override;

    /**
     * @brief Returns the current (relative) bounds of the widget
     */
    Rectangle getBounds() const override;

    /**
     * @brief Changes the current (relative) bounds of the widget
     */
    void setBounds(const Rectangle& bounds) override;

    /**
     * @brief Returns the size (width/height) of the widget
     */
    Dimensions getSize() const override;
    
    /**
     * @brief Handles user input
     * 
     * For button presses, it is advised to track button release situations instead of 
     * button presses for executing an action. Otherwise the key press might be handled again immediately
     * in the next scene/widget because the user wouldn't have had the time to actually release the key.
     */
    bool handleUserInput(const joypad_inputs_t& userInput) override;

    /**
     * @brief Renders the widget
     * 
     * @param gfx The graphics instance that must be used to render the widget
     * @param parentBounds The bounds of the parent widget or scene. You must add the x,y offset of your own bounds 
     * to the parentBounds to get the absolute bounds for rendering.
     * 
     * Getting the parentBounds as an argument of this function was done because a parent widget may be 
     * animated or change positions independent of the child widget. But when the parent widget moves, the child must as well!
     */
    void render(RDPQGraphics& gfx, const Rectangle& parentBounds) override;

    TransferPakWidgetState getState() const;

    /**
     * This function retrieves the gametypes for gen1 and gen2. The hard split is because of the way libpokemegb was set up
     */
    void retrieveGameType(Gen1GameType& outGen1Type, Gen2GameType& outGen2Type);

    void setStyle(const TransferPakDetectionWidgetStyle& style);
    void setStateChangedCallback(void (*callback)(void*, TransferPakWidgetState), void* context);
protected:
private:
    void switchState(TransferPakWidgetState previousState, TransferPakWidgetState newState);

    void renderUnknownState(RDPQGraphics& gfx, const Rectangle& parentBounds);
    void renderErrorState(RDPQGraphics& gfx, const Rectangle& parentBounds);

    /**
     * @brief This function checks every controller for an N64 transfer pak, selects it in our
     * TransferPakManager instance and returns true after it finds the first one.
     */
    bool selectTransferPak();

    /**
     * @brief Checks whether the CRC value of the gameboy header matches.
     * This check is useful for checking whether a good connection with the cartridge was established.
     */
    bool validateGameboyHeader();

    /**
     * @brief This function detects the Pokémon game type in the N64 transfer pak. 
     * It stores the found value in the gen1Type_ and gen2Type_ member vars. These values can be retrieved with retrieveGameType()
     * 
     * @return returns true if the connected game pak is a Gen1 or Gen2 pokémon gameboy game.
     */
    bool detectGameType();

    TransferPakDetectionWidgetStyle style_;
    AnimationManager& animManager_;
    TransferPakManager& tpakManager_;
    Rectangle bounds_;
    Rectangle textBounds_;
    TransferPakWidgetState currentState_;
    joypad_inputs_t previousInputState_;
    Gen1GameType gen1Type_;
    Gen2GameType gen2Type_;
    void (*stateChangedCallback_)(void*, TransferPakWidgetState);
    void* stateChangedCallbackContext_;
    bool focused_;
    bool visible_;
};

#endif