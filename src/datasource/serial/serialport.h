#pragma once

#include <set>
#include <vector>
#include <memory>

#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QComboBox>
#include <QToolButton>
#include <QTimer>
#include <QReadWriteLock>

#include <virtual_terminal_emulator/sequencer.h>
#include <virtual_terminal_emulator/parser/parser.h>

#include <data/point.h>

#include <datasource/data_source_interface.h>
#include <datasource/serial/serialport_config.h>

#include <widget/serialport_control.h>
#include <widget/serialport_terminal.h>

class SerialPortDataSource : public IDataSource, public vte::ISequenceHandler
{
    Q_OBJECT

public:
    SerialPortDataSource(const SerialPortConfig& config, QObject* parent = nullptr);
    virtual ~SerialPortDataSource();

    static QString getName() { return "Serial Port"; }
    virtual bool getNextPoints(std::vector<std::set<Point>>& points) override;
    virtual bool isStream() const override { return true; }
    virtual bool canSignalDataAvailable() const override { return true; }
    virtual QString getTypeName() const override { return "serial"; }
    virtual QWidget* widget() const override;
    virtual void makeTabs(QTabWidget* parent) const override;

    virtual void executeControlCode(char controlCode) override;
    virtual void processSequence(vte::Sequence const& sequence) override;
    virtual void writeText(char32_t codepoint) override;
    virtual void writeText(std::string_view codepoints, std::size_t cellCount) override;

public slots:
    void handlePort(SerialPortConfig);
    void updatePort(SerialPortConfig);

private slots:
    void onSerialReadyRead();
    void onSerialError(QSerialPort::SerialPortError);
    void readLoopTimedOut();

private:
    QTimer* m_readLoopTimeoutTimer = nullptr;
    bool m_readLoopTimeout = false;
    bool m_escapeSequenceFound = false;
    QSerialPort m_serial;
    SerialPortControlWidget* m_widget;
    SerialPortTerminalWidget* m_terminal;

    std::shared_ptr<vte::Sequencer> m_seq;
    std::unique_ptr<vte::parser::Parser> m_parser;

    std::vector<std::set<Point>> m_devicePointsBuffer;
    QReadWriteLock m_bufferLock;
    QString m_lineBuffer;
};

