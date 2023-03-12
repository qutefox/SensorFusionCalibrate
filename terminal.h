#pragma once

#include <QTextEdit>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QString>

#include "ansiescape.h"

class SerialTerminal : public QTextEdit
{
    Q_OBJECT

signals:
    void writeData(QByteArray data);
    void processLine(QString line);

public:
    SerialTerminal(QWidget *parent = nullptr);

    void putData(const QByteArray &data);
    void setAnsiEscape(AnsiEscape* ansiEscape);
    void setLocalEchoEnabled(bool set);

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
    void contextMenuEvent(QContextMenuEvent *e) override;

public slots:
    void scrollToBottom(int state);

private:
    AnsiEscape* m_ansiEscape = nullptr;
    bool m_localEchoEnabled = false;
    bool m_scrollToBottom = true;
    QString m_inputBuffer;
};
