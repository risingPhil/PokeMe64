#ifndef _FONTMANAGER_H
#define _FONTMANAGER_H

#include <libdragon.h>
#include <unordered_map>
#include <string>

typedef struct FontEntry
{
    rdpq_font_t* font;
    uint8_t fontId;
} FontEntry;

typedef std::unordered_map<std::string, FontEntry> RDPQFontMap;

/**
 * @brief This class exists because libdragon does not offer a way to unload a font or otherwise load a font
 * more than once.
 * .
 * Therefore if you want to load the font again in a different scene, you hit an assert.
 * 
 * FontManager prevents this by handling the loading transparently and returning already loaded font handles
 * if the desired font was already loaded before.
 * 
 */
class FontManager
{
public:
    FontManager();

    /**
     * Retrieve a fontId for the font at the given URI
     */
    uint8_t getFont(const char* fontUri);

    /**
     * This function registers the given fontStyle onto the given font and associate it with the specified fontStyleId
     * Note: there's no unregisterFontStyle because libdragon doesn't offer the functionality.
     * 
     * That being said, replacing the fontStyleId is possible without it throwing an assert() in our face
     */
    void registerFontStyle(uint8_t fontId, uint8_t fontStyleId, const rdpq_fontstyle_t& fontStyle);
protected:
private:
    RDPQFontMap fontMap_;
    uint8_t nextFontId_;
};

#endif