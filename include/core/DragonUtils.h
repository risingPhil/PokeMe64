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

extern bool sdcard_mounted;

/**
 * This function determines whether the joypad_inputs_t has analog or dpad positions/presses that could be considered for UI navigation.
 * If so, it will return the most prominent direction.
 */
const UINavigationDirection determineUINavigationDirection(joypad_inputs_t inputs, NavigationInputSourceType sourceType);

/**
 * @brief mounts the SD card (if possible)
 * 
 */
bool mountSDCard();

/**
 * @brief Writes the specified buffer to a file at the specified path.
 * This should be a path starting with sd:/
 * 
 * @param path path starting with sd:/
 * @param buffer buffer to write
 * @param bufferSize size of the buffer
 * @return size_t number of bytes written
 */
size_t writeBufferToFile(const char* path, const uint8_t* buffer, size_t bufferSize);

/**
 * Converts the gb_cart_ram_size_t enum into the actual number of bytes
 */
uint32_t convertSRAMSizeIntoNumBytes(gb_cart_ram_size_t ramSize);

/**
 * This function indicates whether the N64 flashcart we're running from supports
 * access to its SD card
 */
bool doesN64FlashCartSupportSDCardAccess();

#endif