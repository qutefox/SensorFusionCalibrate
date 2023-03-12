#pragma once

#include <QObject>
#include <QRegularExpression>

#include "terminal_data.h"

class TerminalEscape : public QObject
{
    Q_OBJECT

public:
    TerminalEscape(QObject* parent = nullptr);

    UnescapedTerminalData unescape(TerminalData& data);

signals:
    // void updateWindowTitle(QString title);
    // void updateIconName(QString iconName);
    // void updateScreenColorPalette(int paletteIndex, int r, int g, int b);

private:
    const QRegularExpression m_escRegex = QRegularExpression(R"(\x1B(7|8|D|E|H|M|P|\[|\]|\^|_|\\))");

};

