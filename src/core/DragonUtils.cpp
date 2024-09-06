#include "core/DragonUtils.h"
#include "libcart/cart.h"

bool sdcard_mounted = false;

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

bool mountSDCard()
{
    sdcard_mounted = debug_init_sdfs("sd:/", -1);
    return sdcard_mounted;
}

size_t writeBufferToFile(const char* path, const uint8_t* buffer, size_t bufferSize)
{
    size_t ret;
    if(!sdcard_mounted)
    {
        return 0;
    }

    FILE* f = fopen(path, "w");
    if(!f)
    {
        return 0;
    }

    ret = fwrite(buffer, sizeof(char), bufferSize, f);

    fclose(f);
    return ret;
}

uint32_t convertROMSizeIntoNumBytes(gb_cart_rom_size_t romSize)
{
    switch(romSize)
    {
        case GB_ROM_32KB:
            return 32 * 1024;
        case GB_ROM_64KB:
            return 64 * 1024;
        case GB_ROM_128KB:
            return 128 * 1024;
        case GB_ROM_256KB:
            return 256 * 1024;
        case GB_ROM_512KB:
            return 512 * 1024;
        case GB_ROM_1MB:
            return 1024 * 1024;
        case GB_ROM_2MB:
            return 2048 * 1024;
        case GB_ROM_4MB:
            return 4096 * 1024;
        case GB_ROM_8MB:
            return 8192 * 1024;
        case GB_ROM_1152KB:
            return 1152 * 1024;
        case GB_ROM_1280KB:
            return 1280 * 1024;
        case GB_ROM_1536KB:
            return 1536 * 1024;
        default:
            return 0;
    }
}

uint32_t convertSRAMSizeIntoNumBytes(gb_cart_ram_size_t ramSize)
{
    switch(ramSize)
    {
        case GB_RAM_2KB:
            return 2 * 1024;
        case GB_RAM_8KB:
            return 8 * 1024;
        case GB_RAM_32KB:
            return 32 * 1024;
        case GB_RAM_64KB:
            return 64 * 1024;
        case GB_RAM_128KB:
            return 128 * 1024;
        default:
            return 0;
    }
}

bool doesN64FlashCartSupportSDCardAccess()
{
    return (cart_type > CART_NULL && cart_type < CART_MAX);
}