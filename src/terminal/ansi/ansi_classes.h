#pragma once

#include <vector>
#include "ansi_enums.h"

namespace Ansi
{

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


class CursorStyle
{
public:
    CursorShape m_shape = CursorShape_Block;
    bool m_blinking = true;

};

} // namespace Ansi
