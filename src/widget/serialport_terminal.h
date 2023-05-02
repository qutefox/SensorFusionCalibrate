#pragma once

#include <memory>
#include <string>

#include <QGroupBox>
#include <QWidget>
#include <QString>

#include <virtual_terminal_emulator/sequencer.h>
#include <virtual_terminal_emulator/parser/parser.h>

QT_BEGIN_NAMESPACE
namespace Ui { class SerialPortTerminalWidget; }
QT_END_NAMESPACE

class SerialPortTerminalWidget : public QGroupBox, public vte::ISequenceHandler
{
    Q_OBJECT

    virtual void executeControlCode(char controlCode) override;
    virtual void processSequence(vte::Sequence const& sequence) override;
    virtual void writeText(char32_t codepoint) override;
    virtual void writeText(std::string_view codepoints, std::size_t cellCount) override;

    void backspace();
    void moveCursorToNextTab();
    void linefeed();
    void moveCursorToBeginOfLine();

public:
    SerialPortTerminalWidget(QWidget *parent = nullptr);
    virtual ~SerialPortTerminalWidget();\

    void putData(const QByteArray& data);

signals:
    void processLine(const std::string line);

private:
    Ui::SerialPortTerminalWidget* m_ui = nullptr;

    std::shared_ptr<vte::Sequencer> m_seq;
    std::unique_ptr<vte::parser::Parser> m_parser;
    QString m_lineBuffer;
};
