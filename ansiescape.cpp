#include "ansiescape.h"

#include <QDebug>
/*
constexpr char ESC = 0x1B; // escape char
constexpr char CSI = 0x5B; // '[' -> control sequence introducer
constexpr char DCS = 0x50; // 'P' -> device control string
constexpr char OSC = 0x5D;  // ']' -> operating system command
constexpr char BEL = 0x07; // bell character
constexpr char ST2  = 0x5C; // 2nd byte of string terminator. full ST seq: {ESC, ST2}
*/

AnsiEscape::AnsiEscape(QObject* parent)
    : QObject(parent)
{

}

char AnsiEscape::getNextChar(QByteArray& in)
{
    if (in.isEmpty()) return ' ';
    const char c = in.at(0);
    in.remove(0, 1);
    return c;
}

UnescapedData AnsiEscape::unescape(const QByteArray& data)
{
    // qDebug() << data.toHex() << Qt::endl;

    UnescapedData out;
    QByteArray in = data;

    // https://xtermjs.org/docs/api/vtfeatures/
    // https://learn.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
    // https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797

    QRegularExpressionMatch match = m_escRegex.match(QString::fromUtf8(in.constData(), in.size()));
    while(match.hasMatch())
    {
        qsizetype matchLen = 2; // 2 bytes.
        char seq = match.captured(1).toUtf8()[0];
        in.remove(match.capturedStart(), matchLen);
        switch(seq)
        {
        case '7':
            break;
        case '8':
            break;
        case 'D':
            break;
        case 'E':
            break;
        case 'H':
            break;
        case 'M':
            break;
        case 'P':
            break;
        case '[':
            break;
        case ']':
            unescapeOSC(in, out);
            break;
        case '^':
            break;
        case '_':
            break;
        case '\\':
            break;
        };
        match = m_escRegex.match(QString::fromUtf8(in.constData(), in.size()));
    }

    return out;
}

void AnsiEscape::unescapeCSI(QByteArray& in, UnescapedData& out)
{
    // TODO: implement
}

void AnsiEscape::unescapeDCS(QByteArray& in, UnescapedData& out)
{
    // TODO: implement
}

AnsiEscape::OSCMode AnsiEscape::oscStr2Mode(const QString& oscModeStr)
{
    OSCMode oscMode;
    if (oscModeStr == "0")
    {
        oscMode.mode = OSCModeEnum::setIconName | OSCModeEnum::setWindowTitle;
        oscMode.regex = m_oscStringRegex;
    }
    else if (oscModeStr == "1")
    {
        oscMode.mode = OSCModeEnum::setIconName;
        oscMode.regex = m_oscStringRegex;
    }
    else if (oscModeStr == "2")
    {
        oscMode.mode = OSCModeEnum::setWindowTitle;
        oscMode.regex = m_oscStringRegex;
    }
    else if (oscModeStr == "4")
    {
        oscMode.mode = OSCModeEnum::changeColorPalette;
        oscMode.regex = m_oscColorPaletteRegex;
    }
    else if (oscModeStr == "8")
    {
        oscMode.mode = OSCModeEnum::createHyperlink;
        oscMode.regex = m_oscHyperLinkRegex;
    }
    else if (oscModeStr == "10")
    {
        oscMode.mode = OSCModeEnum::setOrQueryDefaultForegroundColor;
        oscMode.regex = m_oscColorOrQueryRegex;
    }
    else if (oscModeStr == "11")
    {
        oscMode.mode = OSCModeEnum::setOrQueryDefaultBackgroundColor;
        oscMode.regex = m_oscColorOrQueryRegex;
    }
    else if (oscModeStr == "12")
    {
        oscMode.mode = OSCModeEnum::setOrQueryDefaultCursorColor;
        oscMode.regex = m_oscColorOrQueryRegex;
    }
    else if (oscModeStr == "104")
    {
        oscMode.mode = OSCModeEnum::resetColorPalette;
        oscMode.regex = m_oscPaletteIndexRegex;
    }
    else if (oscModeStr == "110")
    {
        oscMode.mode = OSCModeEnum::restoreDefaultForegroundColor;
        oscMode.regex = m_oscStrTermRegex;
    }
    else if (oscModeStr == "111")
    {
        oscMode.mode = OSCModeEnum::restoreDefaultBackgroundColor;
        oscMode.regex = m_oscStrTermRegex;
    }
    else if (oscModeStr == "112")
    {
        oscMode.mode = OSCModeEnum::restoreDefaultCursorColor;
        oscMode.regex = m_oscStrTermRegex;
    }
    return oscMode;
}

bool AnsiEscape::getOSCMode(QByteArray& in, OSCMode& oscMode)
{
    QString modeStr;
    while (!in.isEmpty())
    {
        const char c = getNextChar(in);
        if (c == ';' || !std::isdigit(c))
        {
            oscMode = oscStr2Mode(modeStr);
            return oscMode.mode != OSCModeEnum::none;
        }
        else modeStr += c;
    }
    return false;
}

bool AnsiEscape::unescapeOSC(QByteArray& in, UnescapedData& out)
{
    OSCMode oscMode;
    if(!getOSCMode(in, oscMode))
    {
        return false;
    }

    QRegularExpressionMatch match = oscMode.regex.match(QString::fromUtf8(in.constData(), in.size()));
    if (match.hasMatch())
    {
        qsizetype matchLen = match.captured(0).toUtf8().size();
        if (oscMode.mode & OSCModeEnum::setIconName || oscMode.mode & OSCModeEnum::setWindowTitle)
        {
            QString str = match.captured(1);
            // if (str.size() >= 255) Invalid string length?!
            if (oscMode.mode & OSCModeEnum::setIconName) emit updateIconName(str);
            if (oscMode.mode & OSCModeEnum::setWindowTitle) emit updateWindowTitle(str);
        }
        else if (oscMode.mode & OSCModeEnum::changeColorPalette)
        {
            int paletteIndex = match.captured(1).toInt() & 0xFF; // Not sure about index size..
            int r = match.captured(2).toInt() & 0xFF;
            int g = match.captured(3).toInt() & 0xFF;
            int b = match.captured(4).toInt() & 0xFF;
            emit updateScreenColorPalette(paletteIndex, r, g, b);
        }
        else if (oscMode.mode & OSCModeEnum::createHyperlink)
        {
            // QString params = match.captured(1);
            QString uri = match.captured(2);

            /*
            QMap<QString, QString> paramMap;
            for (const QString& param : params.split(":"))
            {
                QList<QString> pair = param.split("=");
                if (pair.size()>=2) paramMap.insert(pair[0], pair[1]);
            }
            */

            out.raw += uri.toUtf8();
            out.formatted += uri.toUtf8();
        }

        in.remove(0, matchLen);

        return true;
    }
    return false;
}
