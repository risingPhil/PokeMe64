#include "core/DragonUtils.h"

static uint8_t ANALOG_STICK_THRESHOLD = 30;

const UINavigationDirection determineUINavigationDirection(joypad_inputs_t inputs, NavigationInputSourceType sourceType)
{
    if(sourceType == NavigationInputSourceType::ANALOG_STICK || sourceType == NavigationInputSourceType::BOTH)
    {
        const int8_t absXVal = static_cast<int8_t>(abs(inputs.stick_x));
        const int8_t absYVal = static_cast<int8_t>(abs(inputs.stick_y));

        if(absXVal > absYVal)
        {
            if(absXVal >= ANALOG_STICK_THRESHOLD)
            {
                return (inputs.stick_x < 0) ? UINavigationDirection::LEFT : UINavigationDirection::RIGHT;
            }
        }
        else
        {
            if(absYVal >= ANALOG_STICK_THRESHOLD)
            {
                return (inputs.stick_y < 0) ? UINavigationDirection::DOWN : UINavigationDirection::UP;
            }
        }
    }

    if(sourceType == NavigationInputSourceType::DPAD || sourceType == NavigationInputSourceType::BOTH)
    {
        if(inputs.btn.d_down)
        {
            return UINavigationDirection::DOWN;
        }
        if(inputs.btn.d_up)
        {
            return UINavigationDirection::UP;
        }
        if(inputs.btn.d_left)
        {
            return UINavigationDirection::LEFT;
        }
        if(inputs.btn.d_right)
        {
            return UINavigationDirection::RIGHT;
        }
    }
    return UINavigationDirection::MAX;
}