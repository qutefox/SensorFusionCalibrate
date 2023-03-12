#pragma once

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QRegularExpression>

class UnescapedData
{
public:
    QByteArray raw;
    QString html;
};

class AnsiEscape : public QObject
{
    Q_OBJECT

public:
    AnsiEscape(QObject* parent = nullptr);

    UnescapedData unescape(const QByteArray& data);

signals:
    void updateWindowTitle(QString title);
    void updateIconName(QString iconName);
    void updateScreenColorPalette(int paletteIndex, int r, int g, int b);

private:
    char getNextChar(QByteArray& in);

    void unescapeCSI(QByteArray& in, UnescapedData& out);
    void unescapeDCS(QByteArray& in, UnescapedData& out);

    enum OSCModeEnum
    {
        none = 0,
        setWindowTitle = 1,
        setIconName = 2,
        changeColorPalette = 4,
        createHyperlink = 8,
        setOrQueryDefaultForegroundColor = 16,
        setOrQueryDefaultBackgroundColor = 32,
        setOrQueryDefaultCursorColor = 64,
        resetColorPalette = 128,
        restoreDefaultForegroundColor = 256,
        restoreDefaultBackgroundColor = 512,
        restoreDefaultCursorColor = 1024
    };

    class OSCMode
    {
    public:
        int mode = OSCModeEnum::none;
        QRegularExpression regex = QRegularExpression(R"((.*?)(\x07|\x1B\x5C))");
    };

    OSCMode oscStr2Mode(const QString& oscModeStr);
    bool getOSCMode(QByteArray& in, OSCMode& oscMode);
    bool unescapeOSC(QByteArray& in, UnescapedData& out);

    const QRegularExpression m_escRegex = QRegularExpression(R"(\x1B?(7|8|D|E|H|M|P|\[|\]|\^|_|\\))");
    const QRegularExpression m_oscColorPaletteRegex = QRegularExpression(R"(([\d]{1,3});rgb:([A-Fa-f0-9]{1,4})\/([A-Fa-f0-9]{1,4})\/([A-Fa-f0-9]{1,4})(\x07|\x1B\x5C))");
    const QRegularExpression m_oscHyperLinkRegex = QRegularExpression(R"((.*?);(.*?)(\x07|\x1B\x5C))");
    const QRegularExpression m_oscStringRegex = QRegularExpression(R"((.*?)(\x07|\x1B\x5C))");
    const QRegularExpression m_oscColorOrQueryRegex = QRegularExpression(R"((rgb:([A-Fa-f0-9]{1,4})\/([A-Fa-f0-9]{1,4})\/([A-Fa-f0-9]{1,4})|\?)(\x07|\x1B\x5C))");
    const QRegularExpression m_oscPaletteIndexRegex = QRegularExpression(R"(([\d]{1,3})(\x07|\x1B\x5C))");
    const QRegularExpression m_oscStrTermRegex = QRegularExpression(R"((\x07|\x1B\x5C))");


};

