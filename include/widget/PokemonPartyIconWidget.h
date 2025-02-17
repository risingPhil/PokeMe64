#ifndef _POKEMONPARTYICONWIDGET_H
#define _POKEMONPARTYICONWIDGET_H

#include "widget/IWidget.h"
#include "core/Sprite.h"
#include "gen1/Gen1Common.h"
#include "gen2/Gen2Common.h"

class IRomReader;
class ISaveManager;

/**
 * This factory class provides a function to obtain a surface_t instance for a given icon.
 * It acts as a form of cache to prevent decoding/allocating the same icon more than once if multiple
 * instances of PokemonPartyIconWidget are created.
 */
class PokemonPartyIconFactory
{
public:
    typedef struct IconMapEntry
    {
        surface_t frame1;
        surface_t frame2;
    } IconMapEntry;

    PokemonPartyIconFactory(IRomReader& romReader);
    ~PokemonPartyIconFactory();

    surface_t getIcon(uint8_t generation, uint8_t specificGenType, uint8_t localization, uint8_t iconType, bool firstFrame);
protected:
private:
    // This is the map of icons. Gen2 has the most icons, that's why I'm using GEN2_ICONTYPE_MAX as the array size
    IconMapEntry iconMap_[(unsigned)Gen2PokemonIconType::GEN2_ICONTYPE_MAX];
    IRomReader& romReader_;
};

typedef struct PokemonPartyIconWidgetStyle
{
    struct {
        /**
         * (optional) background sprite
         */
        sprite_t* sprite;
        /*
        * RenderSettings that influence how the backgroundSprite is
        * being rendered
        */
        SpriteRenderSettings spriteSettings;
    } background;
    struct {
        Rectangle bounds;
        int8_t yOffsetWhenTheresNoFrame2;
    } icon;
    uint8_t fpsWhenFocused;
    uint8_t fpsWhenNotFocused;
} PokemonPartyIconWidgetStyle;

typedef struct PokemonPartyIconWidgetData
{
    PokemonPartyIconFactory* iconFactory;
    /**
     * The generation of the pokemon game (gen 1/2)
     */
    uint8_t generation;
    /**
     * The specific variant of the game
     */
    uint8_t specificGenVersion;

    /**
     * @brief The specific localization of the game
     */
    uint8_t localization;
    /**
     * The pokemon icon type we need to display
     */
    uint8_t iconType;
} PokemonPartyIconWidgetData;

class PokemonPartyIconWidget : public IWidget
{
public:
    PokemonPartyIconWidget();
    virtual ~PokemonPartyIconWidget();

    void setStyle(const PokemonPartyIconWidgetStyle& style);
    void setData(const PokemonPartyIconWidgetData& data);

    /**
     * @brief Returns whether the widget is currently focused
     */
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
protected:
private:
    void reset();

    PokemonPartyIconWidgetStyle style_;
    PokemonPartyIconWidgetData data_;
    surface_t iconFrame1_;
    surface_t iconFrame2_;
    Rectangle bounds_;
    uint32_t frameSwitchTimeoutInTicks_;
    uint64_t nextFrameSwitchTime_;
    bool focused_;
    bool visible_;
    bool showFirstIconFrame_;
};

#endif