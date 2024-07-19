#ifndef _IFOCUSLISTENER_H
#define _IFOCUSLISTENER_H

#include "core/common.h"

class IWidget;

/**
 * @brief Provides metadata on the changed focus
 */
typedef struct FocusChangeStatus
{
    /**
     * @brief These are the bounds of the newly focused widget
     */
    Rectangle focusBounds;
    /**
     * @brief The previously focused widget
     */
    IWidget* prevFocus;
    
    /**
     * @brief The newly focused widget
     */
    IWidget* curFocus;

} FocusChangeStatus;

/**
 * @brief This interface must be implemented by classes that are interested in 
 * getting focus change information from a (Vertical)List widget.
 * 
 * Implementing this interface allows the class to register itself onto a List widget.
 */
class IFocusListener
{
public:
    virtual ~IFocusListener();

    /**
     * @brief This callback function will be triggered by the (Vertical)List widget
     * whenever the user switches the focus of list entries
     */
    virtual void focusChanged(const FocusChangeStatus& status) = 0;
protected:
private:
};

#endif