#include "core/FontManager.h"

FontManager::FontManager()
    : fontMap_()
    , nextFontId_(1)
{
}

uint8_t FontManager::getFont(const char* fontUri)
{
    auto it = fontMap_.find(std::string(fontUri));
    if(it == fontMap_.end())
    {
        FontEntry entry = {
            .font = rdpq_font_load(fontUri),
            .fontId = nextFontId_
        };
        fontMap_.emplace(fontUri, entry);
        ++nextFontId_;

        rdpq_text_register_font(entry.fontId, entry.font);
        return entry.fontId;
    }
    return it->second.fontId;
}

void FontManager::registerFontStyle(uint8_t fontId, uint8_t fontStyleId, const rdpq_fontstyle_t& fontStyle)
{
    for(auto it = fontMap_.begin(); it != fontMap_.end(); ++it)
    {
        if(it->second.fontId == fontId)
        {
            rdpq_font_style(it->second.font, fontStyleId, &fontStyle);
            return;
        }
    }
}