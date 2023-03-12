#pragma once

#include <QByteArray>
#include <QString>

class TerminalData
{
public:
    TerminalData(const QByteArray& data);
    QChar getNextChar();
    const QString& getData() const;
    QString getRemainingData() const;
    void setIndex(qsizetype index);
    qsizetype getIndex() const;
    void reset();

private:
    qsizetype m_index = 0;
    QString m_data;
};

class UnescapedTerminalData
{
public:
    QByteArray raw;
    QString html;
};
