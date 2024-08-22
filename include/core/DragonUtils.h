#ifndef _DRAGONUTILS_H
#define _DRAGONUTILS_H

#include <libdragon.h>

enum class UINavigationDirection
{
    UP,
    RIGHT,
    DOWN,
    LEFT,
    MAX
};

enum class NavigationInputSourceType
{
    NONE,
    ANALOG_STICK,
    DPAD,
    BOTH
};

/**
 * This function determines whether the joypad_inputs_t has analog or dpad positions/presses that could be considered for UI navigation.
 * If so, it will return the most prominent direction.
 */
const UINavigationDirection determineUINavigationDirection(joypad_inputs_t inputs, NavigationInputSourceType sourceType);

#endif