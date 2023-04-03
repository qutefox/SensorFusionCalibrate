#pragma once

#include <QByteArray>

namespace EscapeSequences
{
    constexpr char RIS = 'c'; /** Reset */
    constexpr char IND = 'D'; /** Move cursor down one line in same column.
        If the cursor is at the bottom margin, the screen performs a scroll-up. */
    constexpr char NEL = 'E'; /** Next line. Same as LF. */ // TODO: LF
    constexpr char HTS = 'H'; /** Tabulation set: Set a horizontal tab stop at cursor position. */
    constexpr char RI = 'M'; /** Reverse index: Move cursor up one line in same column. If the
        cursor is at the top margin, the screen performs a scroll-down. */
    constexpr char DECSC = '7'; /** Save cursor: Save cursor position, character attribute (graphic
        rendition), character set, and origin mode selection. */
    constexpr char DECRC = '8'; /** Restore cursor: Restore previously saved cursor position, character
        attribute (graphic rendition), character set, and origin mode
        selection. If none were saved, move cursor to home position.*/
    constexpr char DECALN = '8'; /** Alignment display: Fill screen with uppercase E's for testing
        screen focus and alignment. */
    constexpr char ICH = '@'; /** Insert character*: Insert the indicated # of blank characters. */
    constexpr char CUU = 'A'; /** Cursor up: Move cursor up the indicated # of lines in same column.
        Cursor stops at top margin. */
    constexpr char CUD = 'B'; /** Cursor down: Move cursor down the indicated # of lines in same
        column. Cursor stops at bottom margin. */
    constexpr char CUF = 'C'; /** Cursor forward: Move cursor right the indicated # of columns.
        Cursor stops at right margin. */
    constexpr char CUB = 'D'; /** Cursor back*: Move cursor left the indicated # of columns. Cursor
        stops at left margin. */
    constexpr char CNL = 'E'; /** Cursor next line: Move cursor down the indicated # of lines to
        column 1. */
    constexpr char CPL = 'F'; /** Cursor previous line*: Move cursor up the indicated # of lines to
        column 1. */
    constexpr char CHA = 'G'; /** Cursor horizontal align*: Move cursor to the indicated column in
        current line. */
    constexpr char CUP = 'H'; /** Cursor position: Move cursor to the indicated line, column (origin
        at 1, 1). */
    constexpr char ED = 'J'; /** Erase data (default: from cursor to end of line). */
    constexpr char EL = 'K'; /** Erase in line (default: from cursor to end of line). */
    constexpr char IL = 'L'; /** Insert line: Insert the indicated # of blank lines, starting from
        the current line. Lines displayed below cursor move down. Lines moved
        past the bottom margin are lost. */
    constexpr char DL = 'M'; /** Delete line: Delete the indicated # of lines, starting from the
        current line. As lines are deleted, lines displayed below cursor
        move up. Lines added to bottom of screen have spaces with same
        character attributes as last line move up. */
    constexpr char DCH = 'P'; /** Delete character: Delete the indicated # of characters on the
        current line. When character is deleted, all characters to the right
        of cursor move left. */
    constexpr char ECH = 'X'; /** Erase character: Erase the indicated # of characters on the
        current line. */
    constexpr char HPR = 'a'; /** Horizontal position relative: Same as CUF. */
    constexpr char DA = 'c'; /** Device Attributes. */
    constexpr char VPA = 'd'; /** Vertical position adjust: Move cursor to the indicated line,
        current column. */
    constexpr char VPR = 'e'; /** Vertical position relative: Same as CUD. */
    constexpr char HVP = 'f'; /** Horizontal/Vertical position: Same as CUP. */
    constexpr char TBC = 'g'; /** Tabulation clear: Clears a horizontal tab stop at cursor position. */
    constexpr char SM = 'h'; /** Set mode. */
    constexpr char RM = 'l'; /** Reset mode. */
    constexpr char SGR = 'm'; /** Select graphics rendition: The terminal can display the following
        character attributes that change the character display without
        changing the character (see :mod:`pyte.graphics`). */
    constexpr char DSR  = 'n';
}

class SerialPortUnescape
{
public:
    SerialPortUnescape();

    QString unescape(const QByteArray& in);
};

