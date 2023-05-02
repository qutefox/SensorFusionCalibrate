#include "serialport_terminal.h"
#include "./ui_serialport_terminal.h"

#include <QScrollBar>
#include <QApplication>
#include <QTextCursor>

SerialPortTerminalWidget::SerialPortTerminalWidget(QWidget* parent)
	: QGroupBox(parent)
	, m_ui{ new Ui::SerialPortTerminalWidget }
	, m_seq{ std::make_shared<vte::Sequencer>(*this) }
	, m_parser{ std::make_unique<vte::parser::Parser>(m_seq) }
{
	m_ui->setupUi(this);

	// TODO: find a better way to disallow setTextCursor() changes via mouse clicks.
	m_ui->plainTextEdit->setTextInteractionFlags(Qt::NoTextInteraction);
}

SerialPortTerminalWidget::~SerialPortTerminalWidget()
{
	m_parser.reset();
	m_seq.reset();
}

void SerialPortTerminalWidget::putData(const QByteArray& data)
{
	qDebug() << "serialport_terminal.cpp -> putData: " << data;
	m_parser->parseFragment(data.toStdString());
}

void SerialPortTerminalWidget::backspace()
{
	m_ui->plainTextEdit->textCursor().deletePreviousChar();
}

void SerialPortTerminalWidget::moveCursorToNextTab()
{
	m_ui->plainTextEdit->insertPlainText(QString("\t"));
}

void SerialPortTerminalWidget::linefeed()
{
	QTextCursor tc = m_ui->plainTextEdit->textCursor();
	tc.movePosition(QTextCursor::EndOfLine);
	m_ui->plainTextEdit->setTextCursor(tc);
	m_ui->plainTextEdit->insertPlainText(QString("\n"));

	if (!m_lineBuffer.isEmpty())
	{
		emit processLine(m_lineBuffer.toStdString());
		m_lineBuffer.clear();
	}
}

void SerialPortTerminalWidget::moveCursorToBeginOfLine()
{
	QTextCursor tc = m_ui->plainTextEdit->textCursor();
	tc.movePosition(QTextCursor::StartOfLine);
	m_ui->plainTextEdit->setTextCursor(tc);
}

void SerialPortTerminalWidget::executeControlCode(char controlCode)
{
	qDebug() << "serialport_terminal.cpp -> execute control code: " << controlCode;

	switch (controlCode)
    {
        case 0x00: // NUL
            break;
        case 0x07: // BEL
            QApplication::beep();
            break;
        case 0x08: // BS
            backspace();
            break;
        case 0x09: // TAB
            moveCursorToNextTab();
            break;
        case 0x0A: // LF
			linefeed();
            break;
        case 0x0B: // VT
            // Even though VT means Vertical Tab, it seems that xterm is doing an IND instead.
            [[fallthrough]];
        case 0x0C: // FF
            // Even though FF means Form Feed, it seems that xterm is doing an IND instead.
            //index();
            break;
        // case LS1.finalSymbol: // LS1 (SO)
            // Invokes G1 character set into GL. G1 is designated by a select-character-set (SCS) sequence.
            //_cursor.charsets.lockingShift(CharsetTable::G1);
            break;
        // case LS0.finalSymbol: // LSO (SI)
            // Invoke G0 character set into GL. G0 is designated by a select-character-set sequence (SCS).
            //_cursor.charsets.lockingShift(CharsetTable::G0);
            break;
        case 0x0D:
			moveCursorToBeginOfLine();
			break;
        case 0x37:
			// saveCursor();
			break;
        case 0x38:
			//restoreCursor();
			break;
        default:
            break;
    }
}

void SerialPortTerminalWidget::processSequence(vte::Sequence const& sequence)
{
	vte::FunctionDefinition const* funcSpec = sequence.functionDefinition();
	if (funcSpec != nullptr)
	{
		QString tmpStr;
		switch (*funcSpec)
    	{
			case vte::SETTITLE:
			case vte::SETWINTITLE:
				tmpStr = QString::fromStdString(sequence.intermediateCharacters());
				qDebug() << "serialport_terminal.cpp -> set title: " << tmpStr;
				setTitle(tmpStr);
				break;
			default:
				qDebug() << "serialport_terminal.cpp -> process sequence: unhandled seq. " << *funcSpec;
				break;
		}
	}
}

void SerialPortTerminalWidget::writeText(char32_t codepoint)
{
	QChar ch(codepoint);
	qDebug() << "serialport_terminal.cpp -> writeText: " << ch;
	m_ui->plainTextEdit->insertPlainText(ch);
	m_lineBuffer += ch;

	QScrollBar* bar = m_ui->plainTextEdit->verticalScrollBar();
    bar->setValue(bar->maximum());
}

void SerialPortTerminalWidget::writeText(std::string_view codepoints, std::size_t cellCount)
{
	QString text = QString::fromUtf8(codepoints.data(), cellCount);
	qDebug() << "serialport_terminal.cpp -> writeText: " << text;
	m_ui->plainTextEdit->insertPlainText(text);
	m_lineBuffer += text;

    QScrollBar* bar = m_ui->plainTextEdit->verticalScrollBar();
    bar->setValue(bar->maximum());
}
