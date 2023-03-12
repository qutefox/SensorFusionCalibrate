#include "terminal_escape.h"

TerminalEscape::TerminalEscape(QObject* parent)
    : QObject{ parent }
{

}

UnescapedTerminalData TerminalEscape::unescape(TerminalData& data)
{
    // https://xtermjs.org/docs/api/vtfeatures/
    // https://learn.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
    // https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797

    UnescapedTerminalData out;
    QString seqStr;
    QRegularExpressionMatch match = m_escRegex.match(data.getRemainingData());
    while(match.hasMatch())
    {
        data.setIndex(match.capturedStart(0) + 2); // Escape sequence is 2 bytes long.
        seqStr = match.captured(1);
        switch(seqStr.front().toLatin1())
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
            // todo: unescapeOSC(in, out);
            break;
        case '^':
            break;
        case '_':
            break;
        case '\\':
            break;
        };
        match = m_escRegex.match(data.getRemainingData());
    }
    return out;
}
