#include <QtTest>

#include <string>
#include <memory>

#include "../src/terminal/ansi/ansi.h"

class AnsiTests : public QObject
{
    Q_OBJECT

public:
    AnsiTests();
    ~AnsiTests();

private slots:
    void xParseColorRgbTests();
    void xParseColorRgbNegativeTests();
    void xParseColorLegacyTests();
    void xParseColorLegacyNegativeTests();

};

AnsiTests::AnsiTests()
{

}

AnsiTests::~AnsiTests()
{

}

void AnsiTests::xParseColorRgbTests()
{
    std::string rgbTest1 = "rgb:0/2/F";
    std::shared_ptr<QColor> rgbTest1Color = Ansi::xParseColor(rgbTest1);
    QVERIFY(rgbTest1Color != nullptr);
    QVERIFY(rgbTest1Color->red() == 0);
    QVERIFY(rgbTest1Color->green() == 0x22);
    QVERIFY(rgbTest1Color->blue() == 0xFF);

    std::string rgbTest2 = "rgb:1A/2B/4C";
    std::shared_ptr<QColor> rgbTest2Color = Ansi::xParseColor(rgbTest2);
    QVERIFY(rgbTest2Color != nullptr);
    QVERIFY(rgbTest2Color->red() == 0x1A);
    QVERIFY(rgbTest2Color->green() == 0x2B);
    QVERIFY(rgbTest2Color->blue() == 0x4C);

    std::string rgbTest3 = "rgb:000/5B4/FFF";
    std::shared_ptr<QColor> rgbTest3Color = Ansi::xParseColor(rgbTest3);
    QVERIFY(rgbTest3Color != nullptr);
    QVERIFY(rgbTest3Color->red() == 0);
    QVERIFY(rgbTest3Color->green() == 0x5A);
    QVERIFY(rgbTest3Color->blue() == 0xFF);

    std::string rgbTest4 = "rgb:0000/6B4F/FFFF";
    std::shared_ptr<QColor> rgbTest4Color = Ansi::xParseColor(rgbTest4);
    QVERIFY(rgbTest4Color != nullptr);
    QVERIFY(rgbTest4Color->red() == 0);
    QVERIFY(rgbTest4Color->green() == 0x6A);
    QVERIFY(rgbTest4Color->blue() == 0xFF);
}

void AnsiTests::xParseColorRgbNegativeTests()
{
    std::string rgbTest1 = "rgb:";
    std::shared_ptr<QColor> rgbTest1Color = Ansi::xParseColor(rgbTest1);
    QVERIFY(rgbTest1Color == nullptr);

    std::string rgbTest2 = "rgb:111";
    std::shared_ptr<QColor> rgbTest2Color = Ansi::xParseColor(rgbTest2);
    QVERIFY(rgbTest2Color == nullptr);

    std::string rgbTest3 = "rgb:1/1/11";
    std::shared_ptr<QColor> rgbTest3Color = Ansi::xParseColor(rgbTest3);
    QVERIFY(rgbTest3Color == nullptr);
}

void AnsiTests::xParseColorLegacyTests()
{
    std::string rgbTest1 = "#02F";
    std::shared_ptr<QColor> rgbTest1Color = Ansi::xParseColor(rgbTest1);
    QVERIFY(rgbTest1Color != nullptr);
    QVERIFY(rgbTest1Color->red() == 0);
    QVERIFY(rgbTest1Color->green() == 0x22);
    QVERIFY(rgbTest1Color->blue() == 0xFF);

    std::string rgbTest2 = "#1A2B4C";
    std::shared_ptr<QColor> rgbTest2Color = Ansi::xParseColor(rgbTest2);
    QVERIFY(rgbTest2Color != nullptr);
    QVERIFY(rgbTest2Color->red() == 0x1A);
    QVERIFY(rgbTest2Color->green() == 0x2B);
    QVERIFY(rgbTest2Color->blue() == 0x4C);

    std::string rgbTest3 = "#0005B4FFF";
    std::shared_ptr<QColor> rgbTest3Color = Ansi::xParseColor(rgbTest3);
    QVERIFY(rgbTest3Color != nullptr);
    QVERIFY(rgbTest3Color->red() == 0);
    QVERIFY(rgbTest3Color->green() == 0x5A);
    QVERIFY(rgbTest3Color->blue() == 0xFF);

    std::string rgbTest4 = "#00006B4FFFFF";
    std::shared_ptr<QColor> rgbTest4Color = Ansi::xParseColor(rgbTest4);
    QVERIFY(rgbTest4Color != nullptr);
    QVERIFY(rgbTest4Color->red() == 0);
    QVERIFY(rgbTest4Color->green() == 0x6A);
    QVERIFY(rgbTest4Color->blue() == 0xFF);
}

void AnsiTests::xParseColorLegacyNegativeTests()
{
    std::string rgbTest1 = "#";
    std::shared_ptr<QColor> rgbTest1Color = Ansi::xParseColor(rgbTest1);
    QVERIFY(rgbTest1Color == nullptr);

    std::string rgbTest2 = "#21F1";
    std::shared_ptr<QColor> rgbTest2Color = Ansi::xParseColor(rgbTest2);
    QVERIFY(rgbTest2Color == nullptr);

    std::string rgbTest3 = "#E341910";
    std::shared_ptr<QColor> rgbTest3Color = Ansi::xParseColor(rgbTest3);
    QVERIFY(rgbTest3Color == nullptr);
}

QTEST_APPLESS_MAIN(AnsiTests)

#include "tst_ansi.moc"
