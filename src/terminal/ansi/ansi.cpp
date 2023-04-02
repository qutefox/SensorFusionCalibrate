#include "ansi.h"

#include <cstdint>
#include <cmath>
#include <cctype>



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

Ansi::NamedColor Ansi::NamedColorToBright(NamedColor namedColor)
{
    switch(namedColor)
    {
    case NamedColor_Foreground: return NamedColor_BrightForeground;
    case NamedColor_Black: return NamedColor_BrightBlack;
    case NamedColor_Red: return NamedColor_BrightRed;
    case NamedColor_Green: return NamedColor_BrightGreen;
    case NamedColor_Yellow: return NamedColor_BrightYellow;
    case NamedColor_Blue: return NamedColor_BrightBlue;
    case NamedColor_Magenta: return NamedColor_BrightMagenta;
    case NamedColor_Cyan: return NamedColor_BrightCyan;
    case NamedColor_White: return NamedColor_BrightWhite;
    case NamedColor_DimForeground: return NamedColor_Foreground;
    case NamedColor_DimBlack: return NamedColor_Black;
    case NamedColor_DimRed: return NamedColor_Red;
    case NamedColor_DimGreen: return NamedColor_Green;
    case NamedColor_DimYellow: return NamedColor_Yellow;
    case NamedColor_DimBlue: return NamedColor_Blue;
    case NamedColor_DimMagenta: return NamedColor_Magenta;
    case NamedColor_DimCyan: return NamedColor_Cyan;
    case NamedColor_DimWhite: return NamedColor_White;
    default: return namedColor;
    }
}
Ansi::NamedColor Ansi::NamedColorToDim(NamedColor namedColor)
{
    switch(namedColor)
    {
    case NamedColor_Black: return NamedColor_DimBlack;
    case NamedColor_Red: return NamedColor_DimRed;
    case NamedColor_Green: return NamedColor_DimGreen;
    case NamedColor_Yellow: return NamedColor_DimYellow;
    case NamedColor_Blue: return NamedColor_DimBlue;
    case NamedColor_Magenta: return NamedColor_DimMagenta;
    case NamedColor_Cyan: return NamedColor_DimCyan;
    case NamedColor_White: return NamedColor_DimWhite;
    case NamedColor_Foreground: return NamedColor_DimForeground;
    case NamedColor_BrightBlack: return NamedColor_Black;
    case NamedColor_BrightRed: return NamedColor_Red;
    case NamedColor_BrightGreen: return NamedColor_Green;
    case NamedColor_BrightYellow: return NamedColor_Yellow;
    case NamedColor_BrightBlue: return NamedColor_Blue;
    case NamedColor_BrightMagenta: return NamedColor_Magenta;
    case NamedColor_BrightCyan: return NamedColor_Cyan;
    case NamedColor_BrightWhite: return NamedColor_White;
    case NamedColor_BrightForeground: return NamedColor_Foreground;
    default: return namedColor;
    }
}

char Ansi::mapCharset(StandardCharset charset, char c)
{
    if (charset == StandardCharset_Ascii) return c;
    switch (c) {
    /*
    case '_': return ' ';
    case '`': return '◆';
    case 'a': return '▒';
    case 'b': return '\u{2409}'; // Symbol for horizontal tabulation
    case 'c': return '\u{240c}'; // Symbol for form feed
    case 'd': return '\u{240d}'; // Symbol for carriage return
    case 'e': return '\u{240a}'; // Symbol for line feed
    case 'f': return '°';
    case 'g': return '±';
    case 'h': return '\u{2424}'; // Symbol for newline
    case 'i': return '\u{240b}'; // Symbol for vertical tabulation
    case 'j': return '┘';
    case 'k': return '┐';
    case 'l': return '┌';
    case 'm': return '└';
    case 'n': return '┼';
    case 'o': return '⎺';
    case 'p': return '⎻';
    case 'q': return '─';
    case 'r': return '⎼';
    case 's': return '⎽';
    case 't': return '├';
    case 'u': return '┤';
    case 'v': return '┴';
    case 'w': return '┬';
    case 'x': return '│';
    case 'y': return '≤';
    case 'z': return '≥';
    case '{': return 'π';
    case '|': return '≠';
    case '}': return '£';
    case '~': return '·';
*/
    default:
        return c;
    }
}

