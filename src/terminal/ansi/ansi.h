#pragma once

#include <memory>
#include <vector>
#include <string>

#include <QColor>
#include <QDateTime>
#include <QByteArray>

namespace Ansi
{

std::shared_ptr<QColor> xParseColor(const std::string& in);
std::shared_ptr<QColor> xParseColor(const std::vector<char>& in);

/**
 * @brief Pending DCS (Device Control String) sequence.
 */
enum Dcs
{
    Dcs_None,
    Dcs_SyncStart, /** Begin of the synchronized update. */
    Dcs_SyncEnd /** End of the synchronized update. */
};

class SyncState
{
public:
    std::shared_ptr<QDateTime> m_timeout; /** Expiration time of the synchronized update. */
    Dcs m_pendingDcs = Dcs_None; /** Sync DCS waiting for termination sequence. */
    std::vector<char> m_buffer; /** Bytes read during the synchronized update. */

    SyncState()
        : m_timeout{ nullptr }
        , m_pendingDcs{ Dcs_None }
        , m_buffer{}
    {
        m_buffer.reserve(1024 * 1024 * 2);
    }
};

/**
 * @brief Internal state for VTE processor.
 */
class ProcessorState
{
public:
    char m_precedingChar; /** Last processed character for repetition. */
    Dcs m_dcs; /** DCS sequence waiting for termination. */
    SyncState m_syncState; /** State for synchronized terminal updates. */
};

class Handler
{
public:
    Handler();
};

enum CursorShape
{
    CursorShape_Block, /** Cursor is a block like '▒' */
    CursorShape_Underline, /** Cursor is an underscore like '_'. */
    CursorShape_Beam, /** Cursor is a vertical bar '|'. */
    CursorShape_HollowBlock, /** Cursor is a box like '☐'. */
    CursorShape_Hidden /** Invisible cursor. */
};

class CursorStyle
{
public:
    CursorShape shape = CursorShape_Block;
    bool blinking = true;
};

/**
 * @brief Terminal modes.
 */
enum Mode
{
    /// ?1
    Mode_CursorKeys = 1,
    /// Select 80 or 132 columns per page (DECCOLM).
    ///
    /// CSI ? 3 h -> set 132 column font.
    /// CSI ? 3 l -> reset 80 column font.
    ///
    /// Additionally,
    ///
    /// * set margins to default positions
    /// * erases all data in page memory
    /// * resets DECLRMM to unavailable
    /// * clears data from the status line (if set to host-writable)
    Mode_ColumnMode = 3,
    /// IRM Insert Mode.
    ///
    /// NB should be part of non-private mode enum.
    ///
    /// * `CSI 4 h` change to insert mode
    /// * `CSI 4 l` reset to replacement mode
    Mode_Insert = 4,
    /// ?6
    Mode_Origin = 6,
    /// ?7
    Mode_LineWrap = 7,
    /// ?12
    Mode_BlinkingCursor = 12,
    /// 20
    ///
    /// NB This is actually a private mode. We should consider adding a second
    /// enumeration for public/private modesets.
    Mode_LineFeedNewLine = 20,
    /// ?25
    Mode_ShowCursor = 25,
    /// ?1000
    Mode_ReportMouseClicks = 1000,
    /// ?1002
    Mode_ReportCellMouseMotion = 1002,
    /// ?1003
    Mode_ReportAllMouseMotion = 1003,
    /// ?1004
    Mode_ReportFocusInOut = 1004,
    /// ?1005
    Mode_Utf8Mouse = 1005,
    /// ?1006
    Mode_SgrMouse = 1006,
    /// ?1007
    Mode_AlternateScroll = 1007,
    /// ?1042
    Mode_UrgencyHints = 1042,
    /// ?1049
    Mode_SwapScreenAndSetRestoreCursor = 1049,
    /// ?2004
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

class Performer
{
public:
    Performer();
};

class Processor
{
public:
    Processor();

    void advance(const char c);
    void stopSync();
    std::shared_ptr<QDateTime> syncTimeout();
    std::size_t syncBytesCount();
    void advanceSync(const char c);
    void advanceSyncDcsStart();
    void advanceSyncDcsEnd();

private:
    ProcessorState m_state;

};

} // namespace Ansi
