#include "serialport_terminal.h"

#include <QScrollBar>

SerialPortTerminalWidget::SerialPortTerminalWidget(QWidget* parent)
	: QPlainTextEdit(parent)
	, m_seq{ std::make_shared<vte::Sequencer>(*this) }
	, m_parser{ std::make_unique<vte::parser::Parser>(m_seq) }
{

}

SerialPortTerminalWidget::~SerialPortTerminalWidget()
{

}

void SerialPortTerminalWidget::putData(const QByteArray& data)
{
	m_parser->parseFragment(data.toStdString());
}

void SerialPortTerminalWidget::executeControlCode(char controlCode)
{

}

void SerialPortTerminalWidget::processSequence(vte::Sequence const& sequence)
{
	if(sequence.category() == vte::FunctionCategory::OSC)
	{
		qDebug() << sequence.text();
	}

}

void SerialPortTerminalWidget::writeText(char32_t codepoint)
{
	insertPlainText(QChar(codepoint));

	QScrollBar* bar = verticalScrollBar();
    bar->setValue(bar->maximum());
}

void SerialPortTerminalWidget::writeText(std::string_view codepoints, std::size_t cellCount)
{
	insertPlainText(QString::fromUtf8(codepoints.data(), cellCount));

    QScrollBar* bar = verticalScrollBar();
    bar->setValue(bar->maximum());
}
