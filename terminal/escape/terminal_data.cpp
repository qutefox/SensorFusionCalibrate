#include "terminal_data.h"



TerminalData::TerminalData(const QByteArray& data)
    :  m_index{ 0 }, m_data{ QString::fromUtf8(data.constData(), data.size()) }
{

}

bool TerminalData::hasMoreChar()
{
    return m_data.size() > m_index;
}

QChar TerminalData::getNextChar()
{
    if (m_data.size() <= m_index)
    {
        return '\0';
    }
    return m_data.at(m_index++);
}

const QString& TerminalData::getData() const
{
    return m_data;
}

QString TerminalData::getRemainingData() const
{
    return m_data.sliced(m_index);
}

void TerminalData::setIndex(qsizetype index)
{
    m_index = index;
}

qsizetype TerminalData::getIndex() const
{
    return m_index;
}

void TerminalData::moveIndexByOffset(qsizetype offset)
{
    m_index += offset;
    if (m_index < 0) m_index = 0;
}

void TerminalData::reset()
{
    m_index = 0;
}
