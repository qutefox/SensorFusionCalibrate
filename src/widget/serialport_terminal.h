#pragma once

#include <memory>
#include <QPlainTextEdit>

#include "virtual_terminal_emulator/sequencer.h"
#include "virtual_terminal_emulator/parser/parser.h"

class SerialPortTerminalWidget : public QPlainTextEdit, public vte::ISequenceHandler
{
    Q_OBJECT

    virtual void executeControlCode(char controlCode) override;
    virtual void processSequence(vte::Sequence const& sequence) override;
    virtual void writeText(char32_t codepoint) override;
    virtual void writeText(std::string_view codepoints, std::size_t cellCount) override;

public:
    SerialPortTerminalWidget(QWidget *parent = nullptr);
    virtual ~SerialPortTerminalWidget();

    void putData(const QByteArray& data);

private:
    std::shared_ptr<vte::Sequencer> m_seq;
    std::unique_ptr<vte::parser::Parser> m_parser;

};
