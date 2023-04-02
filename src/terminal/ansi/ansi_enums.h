#pragma once

#include <memory>

namespace Ansi
{

/**
 * @brief Pending DCS (Device Control String) sequence.
 */
enum Dcs
{
    Dcs_None,
    Dcs_SyncStart, /** Begin of the synchronized update. */
    Dcs_SyncEnd /** End of the synchronized update. */
};

/**
 * @brief Terminal cursor shape.
 */
enum CursorShape
{
    CursorShape_Block, /** Cursor is a block like '▒' */
    CursorShape_Underline, /** Cursor is an underscore like '_'. */
    CursorShape_Beam, /** Cursor is a vertical bar '|'. */
    CursorShape_HollowBlock, /** Cursor is a box like '☐'. */
    CursorShape_Hidden /** Invisible cursor. */
};


/**
 * @brief Terminal modes.
 */
enum Mode
{
    // ?1
    Mode_CursorKeys = 1,
    // Select 80 or 132 columns per page (DECCOLM).
    //
    // CSI ? 3 h -> set 132 column font.
    // CSI ? 3 l -> reset 80 column font.
    //
    // Additionally,
    //
    // * set margins to default positions
    // * erases all data in page memory
    // * resets DECLRMM to unavailable
    // * clears data from the status line (if set to host-writable)
    Mode_ColumnMode = 3,
    // IRM Insert Mode.
    //
    // NB should be part of non-private mode enum.
    //
    // * `CSI 4 h` change to insert mode
    // * `CSI 4 l` reset to replacement mode
    Mode_Insert = 4,
    // ?6
    Mode_Origin = 6,
    // ?7
    Mode_LineWrap = 7,
    // ?12
    Mode_BlinkingCursor = 12,
    // 20
    //
    // NB This is actually a private mode. We should consider adding a second
    // enumeration for public/private modesets.
    Mode_LineFeedNewLine = 20,
    // ?25
    Mode_ShowCursor = 25,
    // ?1000
    Mode_ReportMouseClicks = 1000,
    // ?1002
    Mode_ReportCellMouseMotion = 1002,
    // ?1003
    Mode_ReportAllMouseMotion = 1003,
    // ?1004
    Mode_ReportFocusInOut = 1004,
    // ?1005
    Mode_Utf8Mouse = 1005,
    // ?1006
    Mode_SgrMouse = 1006,
    // ?1007
    Mode_AlternateScroll = 1007,
    // ?1042
    Mode_UrgencyHints = 1042,
    // ?1049
    Mode_SwapScreenAndSetRestoreCursor = 1049,
    // ?2004
    Mode_BracketedPaste = 2004,
};

std::shared_ptr<Mode> getMode(const char intermediate, const uint16_t num);

/**
 * @brief Mode for clearing line.
 * Relative to cursor.
 */
enum LineClearMode
{
    LineClearMode_Right, /** Clear right of cursor. */
    LineClearMode_Left, /** Clear left of cursor. */
    LineClearMode_All /** Clear entire line. */
};

enum ClearMode
{
    ClearMode_Below, /** Clear below cursor. */
    ClearMode_Above, /** Clear above cursor. */
    ClearMode_All, /** Clear entire terminal. */
    ClearMode_Saved, /** Clear 'saved' lines (scrollback). */
};

/**
 * @brief Mode for clearing tab stops.
 */
enum TabulationClearMode
{
    TabulationClearMode_Current, /** Clear stop under cursor. */
    TabulationClearMode_All, /** Clear all stops. */
};

/**
 * @brief Standard colors.
 *
 * The order here matters since the enum should be castable to a 'uint' for
 * indexing a color list.
 */
enum NamedColor
{
    NamedColor_Black = 0, /** Black. */
    NamedColor_Red, /** Red. */
    NamedColor_Green, /** Green. */
    NamedColor_Yellow, /** Yellow. */
    NamedColor_Blue, /** Blue. */
    NamedColor_Magenta, /** Magenta. */
    NamedColor_Cyan, /** Cyan. */
    NamedColor_White, /** White. */
    NamedColor_BrightBlack, /** Bright black. */
    NamedColor_BrightRed, /** Bright red. */
    NamedColor_BrightGreen, /** Bright green. */
    NamedColor_BrightYellow, /** Bright yellow. */
    NamedColor_BrightBlue, /** Bright blue. */
    NamedColor_BrightMagenta, /** Bright magenta. */
    NamedColor_BrightCyan, /** Bright cyan. */
    NamedColor_BrightWhite, /** Bright white. */
    NamedColor_Foreground = 256, /** The foreground color. */
    NamedColor_Background, /** The background color. */
    NamedColor_Cursor, /** Color for the cursor itself. */
    NamedColor_DimBlack, /** Dim black. */
    NamedColor_DimRed, /** Dim red. */
    NamedColor_DimGreen, /** Dim green. */
    NamedColor_DimYellow, /** Dim yellow. */
    NamedColor_DimBlue, /** Dim blue. */
    NamedColor_DimMagenta, /** Dim magenta. */
    NamedColor_DimCyan, /** Dim cyan. */
    NamedColor_DimWhite, /** Dim white. */
    NamedColor_BrightForeground, /** The bright foreground color. */
    NamedColor_DimForeground /** Dim foreground. */
};

NamedColor NamedColorToBright(NamedColor namedColor);
NamedColor NamedColorToDim(NamedColor namedColor);

enum Color
{
    Color_Named, // NamedColor
    Spec, // Rgb (QColor)
    Indexed // a byte
};

/**
 * @brief Terminal character attributes.
 */
enum Attr
{
    Attr_Reset, /** Clear all special abilities. */
    Attr_Bold, /** Bold text. */
    Attr_Dim, /** Dim or secondary color. */
    Attr_Italic, /** Italic text. */
    Attr_Underline, /** Underline text. */
    Attr_DoubleUnderline, /** Underlined twice. */
    Attr_Undercurl, /** Undercurled text. */
    Attr_DottedUnderline, /** Dotted underlined text. */
    Attr_DashedUnderline, /** Dashed underlined text. */
    Attr_BlinkSlow, /** Blink cursor slowly. */
    Attr_BlinkFast, /** Blink cursor fast. */
    Attr_Reverse, /** Invert colors. */
    Attr_Hidden, /** Do not display characters. */
    Attr_Strike, /** Strikeout text. */
    Attr_CancelBold, /** Cancel bold. */
    Attr_CancelBoldDim, /** Cancel bold and dim. */
    Attr_CancelItalic, /** Cancel italic. */
    Attr_CancelUnderline, /** Cancel all underlines. */
    Attr_CancelBlink, /** Cancel blink. */
    Attr_CancelReverse, /** Cancel inversion. */
    Attr_CancelHidden, /** Cancel text hiding. */
    Attr_CancelStrike, /** Cancel strikeout. */
    Attr_Foreground, /** Set indexed foreground color. */
    Attr_Background, /** Set indexed background color. */
    Attr_UnderlineColor, /** Underline color. */
};

/**
 * @brief Identifiers which can be assigned to a graphic character set.
 */
enum CharsetIndex
{
    CharsetIndex_G0, // default
    CharsetIndex_G1,
    CharsetIndex_G2,
    CharsetIndex_G3
};

/**
 * @brief Standard or common character sets which can be designated as G0-G3.
 */
enum StandardCharset
{
    StandardCharset_Ascii, // default
    StandardCharset_SpecialCharacterAndLineDrawing,
};

} // namespace Ansi
