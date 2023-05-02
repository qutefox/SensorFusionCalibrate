#include "serialport_terminal.h"

#include <QScrollBar>
#include <QApplication>
#include <QTextCursor>

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
	qDebug() << "serialport_terminal.cpp -> putData: " << data;
	m_parser->parseFragment(data.toStdString());
}

void SerialPortTerminalWidget::backspace()
{
	textCursor().deletePreviousChar();
}

void SerialPortTerminalWidget::moveCursorToNextTab()
{
	insertPlainText(QString("\t"));
}

void SerialPortTerminalWidget::linefeed()
{
	QTextCursor tc = textCursor();
	tc.movePosition(QTextCursor::EndOfLine);
	setTextCursor(tc);
	insertPlainText(QString("\n"));
}

void SerialPortTerminalWidget::moveCursorToBeginOfLine()
{
	QTextCursor tc = textCursor();
	tc.movePosition(QTextCursor::StartOfLine);
	setTextCursor(tc);
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
				// TODO: setTitle(tmpStr);
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
	insertPlainText(ch);

	QScrollBar* bar = verticalScrollBar();
    bar->setValue(bar->maximum());
}

void SerialPortTerminalWidget::writeText(std::string_view codepoints, std::size_t cellCount)
{
	QString text = QString::fromUtf8(codepoints.data(), cellCount);
	qDebug() << "serialport_terminal.cpp -> writeText: " << text;
	insertPlainText(text);

    QScrollBar* bar = verticalScrollBar();
    bar->setValue(bar->maximum());
}
