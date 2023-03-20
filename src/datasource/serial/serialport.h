#pragma once

#include <set>

#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QComboBox>
#include <QToolButton>
#include <QTimer>

#include <datasource/data_source_interface.h>
#include <widget/serialport_control.h>

class SerialPortDataSource : public IDataSource
{
    Q_OBJECT

public:
    SerialPortDataSource(const SerialPortConfig& config, QObject* parent = nullptr);
    virtual ~SerialPortDataSource();

    static QString getName() { return "Serial Port"; }
    bool getNextPoints(std::vector<std::set<Point>>& points) override;
    bool isStream() const override { return true; }
    QWidget* widget() const override;

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
    QSerialPort m_serial;
    SerialPortControlWidget* m_widget;

};

