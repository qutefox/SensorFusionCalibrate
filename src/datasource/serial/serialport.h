#pragma once

#include <set>

#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QComboBox>
#include <QToolButton>
#include <QTimer>

#include <datasource/data_source_interface.h>
#include <datasource/serial/serialport_config.h>
#include <widget/serialport_control.h>
#include <widget/serialport_terminal.h>

class SerialPortDataSource : public IDataSource
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

public slots:
    void handlePort(SerialPortConfig);
    void updatePort(SerialPortConfig);

private slots:
    void onSerialReadyRead();
    void onSerialError(QSerialPort::SerialPortError);
    void readLoopTimedOut();

private:
    QTimer* m_timer = nullptr;
    bool m_readLoopTimeout = false;
    bool m_escapeSequenceFound = false;
    QSerialPort m_serial;
    SerialPortControlWidget* m_widget;
    SerialPortTerminalWidget* m_terminal;
};

