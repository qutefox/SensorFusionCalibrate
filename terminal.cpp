#include "terminal.h"
#include "qevent.h"

#include <QScrollBar>

SerialTerminal::SerialTerminal(QWidget *parent) :
    QTextEdit(parent), m_ansiEscape{nullptr}
{
    document()->setMaximumBlockCount(100);
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::white);
    setPalette(p);
}

void SerialTerminal::setAnsiEscape(AnsiEscape* ansiEscape)
{
    m_ansiEscape = ansiEscape;
}

void SerialTerminal::putData(const QByteArray &_data)
{
    QByteArray rawData = _data;
    QByteArray formattedData = _data;

    if (m_ansiEscape != nullptr)
    {
        UnescapedData data = m_ansiEscape->unescape(_data);
        rawData = data.raw;
        formattedData = data.formatted;
    }

    insertPlainText(QString::fromUtf8(formattedData.constData(), formattedData.size()));

    if (m_scrollToBottom)
    {
        QScrollBar *bar = verticalScrollBar();
        bar->setValue(bar->maximum());
    }

    m_inputBuffer += QString(rawData);

    if (!m_inputBuffer.isEmpty())
    {
        QString line;
        for (const QChar c :  qAsConst(m_inputBuffer)) // similar to: std::as_const()
        {
            if (c == '\r' || c == '\n')
            {
                if (!line.isEmpty())
                {
                    emit processLine(line);
                    line.clear();
                }
            }
            else line.append(c);
        }
        m_inputBuffer = line; // remaining line
    }

}

void SerialTerminal::setLocalEchoEnabled(bool set)
{
    m_localEchoEnabled = set;
}

void SerialTerminal::scrollToBottom(int state)
{
    if (state == Qt::Checked) m_scrollToBottom = true;
    else m_scrollToBottom = false;
}

void SerialTerminal::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    // TODO: not sure about these keys, maybe we should asni escape send them?
    case Qt::Key_Backspace:
        qDebug() << e->text().toLocal8Bit() << Qt::endl;
        qDebug() << QByteArray::fromHex("08") << Qt::endl;
        emit writeData(QByteArray::fromHex("08"));
        break;
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Up:
    case Qt::Key_Down:

        break;
    default:
        if (m_localEchoEnabled)
            QTextEdit::keyPressEvent(e);
        emit writeData(e->text().toLocal8Bit());
    }
}

void SerialTerminal::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    setFocus();
}

void SerialTerminal::mouseDoubleClickEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
}

void SerialTerminal::contextMenuEvent(QContextMenuEvent *e)
{
    Q_UNUSED(e);
}
