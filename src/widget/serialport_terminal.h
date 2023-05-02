#pragma once

#include <memory>
#include <string>

#include <QGroupBox>
#include <QWidget>

#include <virtual_terminal_emulator/sequencer.h>

QT_BEGIN_NAMESPACE
namespace Ui { class SerialPortTerminalWidget; }
QT_END_NAMESPACE

class SerialPortTerminalWidget : public QGroupBox, public vte::ISequenceHandler
{
    Q_OBJECT

    void backspace();
    void moveCursorToNextTab();
    void linefeed();
    void moveCursorToBeginOfLine();

public:
    SerialPortTerminalWidget(QWidget *parent = nullptr);
    virtual ~SerialPortTerminalWidget();

    virtual void executeControlCode(char controlCode) override;
    virtual void processSequence(vte::Sequence const& sequence) override;
    virtual void writeText(char32_t codepoint) override;
    virtual void writeText(std::string_view codepoints, std::size_t cellCount) override;

private:
    Ui::SerialPortTerminalWidget* m_ui = nullptr;
};
