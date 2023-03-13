#include "escape_osc.h"

const QRegularExpression m_oscColorPaletteRegex = QRegularExpression(R"(([\d]{1,3});rgb:([A-Fa-f0-9]{1,4})\/([A-Fa-f0-9]{1,4})\/([A-Fa-f0-9]{1,4})(\x07|\x1B\x5C))");
const QRegularExpression m_oscHyperLinkRegex = QRegularExpression(R"((.*?);(.*?)(\x07|\x1B\x5C))");
const QRegularExpression m_oscStringRegex = QRegularExpression(R"((.*?)(\x07|\x1B\x5C))");
const QRegularExpression m_oscColorOrQueryRegex = QRegularExpression(R"((rgb:([A-Fa-f0-9]{1,4})\/([A-Fa-f0-9]{1,4})\/([A-Fa-f0-9]{1,4})|\?)(\x07|\x1B\x5C))");
const QRegularExpression m_oscPaletteIndexRegex = QRegularExpression(R"(([\d]{1,3})(\x07|\x1B\x5C))");
const QRegularExpression m_oscStrTermRegex = QRegularExpression(R"(^(\x07|\x1B\x5C))");

using namespace Escape::OperatingSystemCommand;

OSCData oscStr2Mode(const QString& oscModeStr)
{
    OSCData oscData;
    if (oscModeStr == "0")
    {
        oscData.mode = OSCMode::setWindowTitleAndIconName;
        oscData.regex = m_oscStringRegex;
    }
    else if (oscModeStr == "1")
    {
        oscData.mode = OSCMode::setIconName;
        oscData.regex = m_oscStringRegex;
    }
    else if (oscModeStr == "2")
    {
        oscData.mode = OSCMode::setWindowTitle;
        oscData.regex = m_oscStringRegex;
    }
    else if (oscModeStr == "4")
    {
        oscData.mode = OSCMode::changeColorPalette;
        oscData.regex = m_oscColorPaletteRegex;
    }
    else if (oscModeStr == "8")
    {
        oscData.mode = OSCMode::createHyperlink;
        oscData.regex = m_oscHyperLinkRegex;
    }
    else if (oscModeStr == "10")
    {
        oscData.mode = OSCMode::setOrQueryDefaultForegroundColor;
        oscData.regex = m_oscColorOrQueryRegex;
    }
    else if (oscModeStr == "11")
    {
        oscData.mode = OSCMode::setOrQueryDefaultBackgroundColor;
        oscData.regex = m_oscColorOrQueryRegex;
    }
    else if (oscModeStr == "12")
    {
        oscData.mode = OSCMode::setOrQueryDefaultCursorColor;
        oscData.regex = m_oscColorOrQueryRegex;
    }
    else if (oscModeStr == "104")
    {
        oscData.mode = OSCMode::resetColorPalette;
        oscData.regex = m_oscPaletteIndexRegex;
    }
    else if (oscModeStr == "110")
    {
        oscData.mode = OSCMode::restoreDefaultForegroundColor;
        oscData.regex = m_oscStrTermRegex;
    }
    else if (oscModeStr == "111")
    {
        oscData.mode = OSCMode::restoreDefaultBackgroundColor;
        oscData.regex = m_oscStrTermRegex;
    }
    else if (oscModeStr == "112")
    {
        oscData.mode = OSCMode::restoreDefaultCursorColor;
        oscData.regex = m_oscStrTermRegex;
    }
    return oscData;
}

bool getOSCMode(TerminalData& in, OSCData& oscData)
{
    oscData = OSCData(); // reset
    QString modeStr;
    while (in.hasMoreChar())
    {
        const QChar c = in.getNextChar();
        if (c == ';' || !c.isDigit())
        {
            oscData = oscStr2Mode(modeStr);
            return oscData.mode != OSCMode::none;
        }
        else modeStr += c;
    }
    return false;
}

bool unescapeOperatingSystemCommand(TerminalData& in, OSCData& oscData)
{
    if (!getOSCMode(in, oscData)) return false;

    QRegularExpressionMatch match = oscData.regex.match(in.getRemainingData());
    if(!match.hasMatch()) return false;

    qsizetype matchLen = match.captured(0).size();

    if (oscData.mode == OSCMode::setWindowTitleAndIconName)
    {
        oscData.title = oscData.iconName = match.captured(1);
    }
    else if (oscData.mode == OSCMode::setIconName)
    {
        oscData.iconName = match.captured(1);
    }
    else if (oscData.mode == OSCMode::setWindowTitle)
    {
        oscData.title = match.captured(1);
    }
    else if (oscData.mode == OSCMode::changeColorPalette)
    {
        // TODO: change regex for ? matching.
        // TODO: allow multiple c;spec sequence in one command.
        if (match.captured(2) == "?")
        {
            PaletteColorData colorData(match.captured(1));
            oscData.color = colorData;
            oscData.query = true;
        }
        else
        {
            PaletteColorData colorData(match.captured(1), match.captured(2), match.captured(3), match.captured(4));
            oscData.color = colorData;
        }
    }
    else if (oscData.mode == OSCMode::createHyperlink)
    {
        QString params = match.captured(1);
        oscData.uri = match.captured(2);

        for (const QString& param : params.split(":"))
        {
            QList<QString> pair = param.split("=");
            if (pair.size()>=2) oscData.uriParams.insert(pair[0], pair[1]);
        }
    }
    else if (oscData.mode == OSCMode::setOrQueryDefaultForegroundColor)
    {

    }

    in.moveIndexByOffset(matchLen);
    return true;
}

