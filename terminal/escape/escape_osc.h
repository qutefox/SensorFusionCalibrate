#pragma once

#include "terminal_data.h"

#include <QMap>
#include <QRegularExpression>

namespace Escape
{
namespace OperatingSystemCommand
{

    enum OSCMode
    {
        none = -1,
        setWindowTitleAndIconName = 0,
        setIconName = 1,
        setWindowTitle = 2,
        changeColorPalette = 4,
        createHyperlink = 8,
        setOrQueryDefaultForegroundColor = 10,
        setOrQueryDefaultBackgroundColor = 11,
        setOrQueryDefaultCursorColor = 12,
        resetColorPalette = 104,
        restoreDefaultForegroundColor = 110,
        restoreDefaultBackgroundColor = 111,
        restoreDefaultCursorColor = 112
    };

    class PaletteColorData
    {
        int m_paletteIndex = 0;
        int m_r = 0;
        int m_g = 0;
        int m_b = 0;
        bool m_query = false;

        int parse(const QString& color) const
        {
            if (color.size() > 4) return 0;
            int max = pow(16, color.size()) - 1;
            bool ok = false;
            int value = color.toInt(&ok, 16);
            if (!ok) return 0;
            return 255 * value / max;
        }

    public:
        PaletteColorData(int paletteIndex, int r, int g, int b)
            : m_paletteIndex{ paletteIndex & 0xFF }
            , m_r{ r & 0xFF }
            , m_g{ g & 0xFF }
            , m_b{ b & 0xFF }
        { }

        PaletteColorData(int paletteIndex)
            : m_paletteIndex{ paletteIndex }
            , m_query{ true }
        { }

        PaletteColorData(const QString& c, const QString& r, const QString& g, const QString& b)
            : m_paletteIndex{ c.toInt() }
            , m_r{ parse(r) }
            , m_g{ parse(g) }
            , m_b{ parse(b) }
        { }

        PaletteColorData(const QString& c)
            : m_paletteIndex{ c.toInt() }
            , m_query{ true }
        { }

        int getPaletteIndex() const { return m_paletteIndex; }
        int getR() const { return m_r; }
        int getG() const { return m_g; }
        int getB() const { return m_b; }
        bool isQuery() const { return m_query; }

    };

    class OSCData
    {
    public:
        int mode = OSCMode::none;
        QRegularExpression regex = QRegularExpression(R"((.*?)(\x07|\x1B\x5C))");
        PaletteColorData color;
        QString uri, title, iconName;
        QMap<QString, QString> uriParams;
        bool query = false;
    };

    bool unescapeOperatingSystemCommand(TerminalData& in, OSCData& oscData);

    QString escapeSetWindowTitleAndIconName(const QString& str)
    {
        return QString("\x1B]0;%1\x1B\x5C").arg(str);
    }

    QString escapeSetIconName(const QString& str)
    {
        return QString("\x1B]1;%1\x1B\x5C").arg(str);
    }

    QString escapeSetWindowTitle(const QString& str)
    {
        return QString("\x1B]2;%1\x1B\x5C").arg(str);
    }

    QString escapeChangeColorPalette(const PaletteColorData& data)
    {
        if (data.isQuery())
        {
            return QString("\x1B]4;%1;?\x1B\x5C")
                .arg(data.getPaletteIndex());
        }
        else
        {
            return QString("\x1B]4;%1;rgb:%2/%3/%4\x1B\x5C")
                .arg(data.getPaletteIndex())
                .arg(data.getR(),2,16).arg(data.getG(),2,16).arg(data.getB(),2,16);
        }
    }

} // namespace OperatingSystemCommand
} // namespace Escape
