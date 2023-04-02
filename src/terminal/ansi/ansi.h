#pragma once

#include <memory>
#include <vector>
#include <string>

#include <QObject>
#include <QDateTime>
#include <QByteArray>

#include "ansi_color.h"
#include "ansi_enums.h"
#include "ansi_classes.h"

namespace Ansi
{




/**
 * @brief Switch/Map character to the active charset. Ascii is the common case and
 *     for that we want to do as little as possible.
 * @param charset
 * @param c
 * @return
 */
QChar mapCharset(StandardCharset charset, QChar c);

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
