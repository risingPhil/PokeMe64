#include "version.h"

#include <cstring>
#include <cstdio>
#include <cstdint>

#ifdef DEV_BUILD
static const char DEV_SUFFIX_STRING[] = "-dev";

static const char *months[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

static uint8_t get_month_number(const char *month)
{
    for (uint8_t i = 0; i < 12; ++i)
    {
        if (strcmp(month, months[i]) == 0)
        {
            return i + 1;
        }
    }
    return 0; // Should never happen
}
#endif

const char* getPokeMe64VersionString()
{
    static char versionString[20];

    memcpy(versionString, POKEME64_VERSION_NUMBER, sizeof(POKEME64_VERSION_NUMBER));

#ifdef DEV_BUILD
    size_t currentStringLength;
    char month[4];
    uint16_t year;
    int day;
    uint8_t monthNumber;

    strcat(versionString, DEV_SUFFIX_STRING);
    // -2 to not count the null terminators
    currentStringLength = sizeof(POKEME64_VERSION_NUMBER) + sizeof(DEV_SUFFIX_STRING) - 2;

    // Parse __DATE__ ("MMM DD YYYY")
    // We can't use strptime() because libdragon doesn't appear to have it.
    sscanf(__DATE__, "%3s %d %hu", month, &day, &year);
    monthNumber = get_month_number(month);
    snprintf(versionString + currentStringLength, sizeof(versionString) - currentStringLength, "%04hu%02hhu%02d", year, monthNumber, day);
#endif
    return versionString;
}