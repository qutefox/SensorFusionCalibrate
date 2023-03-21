#include "ansi.h"

#include <cstdint>
#include <cmath>
#include <cctype>

std::shared_ptr<QColor> parseColorParts(const std::string& r, const std::string& g, const std::string& b)
{
    if (r.empty() || r.size() != g.size() || r.size() != b.size()) return nullptr;

    std::size_t maxValue = std::pow(16, r.size()) - 1;

    unsigned int rValue = std::stoi(r, 0, 16);
    uint8_t rInt = 255 * rValue / maxValue;

    unsigned int gValue = std::stoi(g, 0, 16);
    uint8_t gInt = 255 * gValue / maxValue;

    unsigned int bValue = std::stoi(b, 0, 16);
    uint8_t bInt = 255 * bValue / maxValue;

    return std::make_shared<QColor>(rInt, gInt, bInt);
}

std::shared_ptr<QColor> parseLegacyColor(const std::vector<char>& in, std::size_t offset)
{
    // Parse colors in '#r(rrr)g(ggg)b(bbb)' format.
    std::vector<char> color;
    color.reserve(12);
    for ( ; offset < in.size() ; ++offset)
    {
        const char c = in[offset];
        if (std::isxdigit(c) && color.size() < 12)
        {
            color.push_back(c);
        }
        else break;
    }
    unsigned short itemLength = color.size() / 3;
    if (itemLength * 3 != color.size()) return nullptr;

    return parseColorParts(
        std::string(color.data(), itemLength),
        std::string(&color[itemLength], itemLength),
        std::string(&color[itemLength*2], itemLength)
    );
}

std::shared_ptr<QColor> parseRgbColor(const std::vector<char>& in, std::size_t offset)
{
    // Parse colors in 'rgb:r(rrr)/g(ggg)/b(bbb)' format.
    std::vector<char> r, g, b;
    r.reserve(4);
    g.reserve(4);
    b.reserve(4);
    unsigned short characterCountPerColor = 0;
    bool characterCountDecided = false;
    unsigned short characterIndex = 0;
    char color = 'r';
    for ( ; offset < in.size() ; ++offset)
    {
        const char c = in[offset];
        if (c == '/')
        {
            if (!characterCountDecided)
            {
                if (characterIndex > 4)
                {
                    // Maximum 4 characters are allowed.
                    return nullptr;
                }
                characterCountPerColor = characterIndex;
                characterCountDecided = true;
            }
            if (color == 'r') color = 'g';
            else if (color == 'g') color = 'b';
            else break;
            characterIndex = 0;
        }
        else if (characterCountDecided && characterIndex > characterCountPerColor)
        {
            if (color == 'b') break; // End of input.

            // All color parts must have the same length!
            return nullptr;
        }
        else
        {
            if (color == 'r') r.push_back(c);
            else if (color == 'g') g.push_back(c);
            else if (color == 'b') b.push_back(c);
            ++characterIndex;
        }
    }

    return parseColorParts(
        std::string(r.data(), r.size()),
        std::string(g.data(), g.size()),
        std::string(b.data(), b.size())
    );
}

std::shared_ptr<QColor> Ansi::xParseColor(const std::string& in)
{
    return xParseColor(std::vector<char>(in.begin(), in.end()));
}

std::shared_ptr<QColor> Ansi::xParseColor(const std::vector<char>& in)
{
    if (!in.empty() && in[0] == '#')
    {
        return parseLegacyColor(in, 1);
    }
    else if (in.size() >= 4 && std::string(in.data(), 4) == "rgb:")
    {
        return parseRgbColor(in, 4);
    }
    return nullptr;
}

std::shared_ptr<Ansi::Mode> Ansi::getMode(const char intermediate, const uint16_t num)
{
    if (intermediate == '?')
    {
        switch(num)
        {
        case 1: return std::make_shared<Mode>(Mode_CursorKeys);
        case 3: return std::make_shared<Mode>(Mode_ColumnMode);
        case 6: return std::make_shared<Mode>(Mode_Origin);
        case 7: return std::make_shared<Mode>(Mode_LineWrap);
        case 12: return std::make_shared<Mode>(Mode_BlinkingCursor);
        case 25: return std::make_shared<Mode>(Mode_ShowCursor);
        case 1000: return std::make_shared<Mode>(Mode_ReportMouseClicks);
        case 1002: return std::make_shared<Mode>(Mode_ReportCellMouseMotion);
        case 1003: return std::make_shared<Mode>(Mode_ReportAllMouseMotion);
        case 1004: return std::make_shared<Mode>(Mode_ReportFocusInOut);
        case 1005: return std::make_shared<Mode>(Mode_Utf8Mouse);
        case 1006: return std::make_shared<Mode>(Mode_SgrMouse);
        case 1007: return std::make_shared<Mode>(Mode_AlternateScroll);
        case 1042: return std::make_shared<Mode>(Mode_UrgencyHints);
        case 1049: return std::make_shared<Mode>(Mode_SwapScreenAndSetRestoreCursor);
        case 2004: return std::make_shared<Mode>(Mode_BracketedPaste);
        }
    }
    else
    {
        switch(num)
        {
        case 4: return std::make_shared<Mode>(Mode_Insert);
        case 20: return std::make_shared<Mode>(Mode_LineFeedNewLine);
        }
    }
    return nullptr;
}
