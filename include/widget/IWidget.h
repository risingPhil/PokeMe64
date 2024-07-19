#ifndef _IWIDGET_H
#define _IWIDGET_H

#include "core/common.h"
#include <libdragon.h>

class RDPQGraphics;

/**
 * This interface class will be used for every UI widget on screen to expose common APIs
 */
class IWidget
{
public:
    /**
     * @brief Returns whether the widget is currently focused
     */
    virtual bool isFocused() const = 0;
    
    /**
     * @brief Sets whether the widget is currently focused
     * 
     */
    virtual void setFocused(bool isFocused) = 0;

    /**
     * @brief Returns whether the widget is currently visible
     */
    virtual bool isVisible() const = 0;
    
    /**
     * @brief Changes the visibility of the widget
     */
    virtual void setVisible(bool visible) = 0;

    /**
     * @brief Returns the current (relative) bounds of the widget
     */
    virtual Rectangle getBounds() const = 0;

    /**
     * @brief Changes the current (relative) bounds of the widget
     */
    virtual void setBounds(const Rectangle& bounds) = 0;

    /**
     * @brief Returns the size (width/height) of the widget
     */
    virtual Dimensions getSize() const = 0;
    
    /**
     * @brief Handles user input
     * 
     * For button presses, it is advised to track button release situations instead of 
     * button presses for executing an action. Otherwise the key press might be handled again immediately
     * in the next scene/widget because the user wouldn't have had the time to actually release the key.
     */
    virtual bool handleUserInput(const joypad_inputs_t& userInput) = 0;

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
    virtual void render(RDPQGraphics& gfx, const Rectangle& parentBounds) = 0;
protected:
private:
};

#endif
