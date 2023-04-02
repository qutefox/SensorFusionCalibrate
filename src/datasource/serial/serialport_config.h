#pragma once

#include <QSerialPort>

class SerialPortConfig
{
public:
    QString m_portName;
    QString m_autoConnectPortId;
    unsigned int m_baudRate = 115200;
    QSerialPort::DataBits m_dataBits = QSerialPort::DataBits::Data8;
    QSerialPort::Parity m_parity = QSerialPort::Parity::NoParity;
    QSerialPort::StopBits m_stopBits = QSerialPort::StopBits::OneStop;
    QSerialPort::FlowControl m_flowControl = QSerialPort::FlowControl::NoFlowControl;

    SerialPortConfig();
    SerialPortConfig(const QString& portName, const QString& autoConnectPortId,
        unsigned int baudRate, QSerialPort::DataBits dataBits, QSerialPort::Parity parity,
        QSerialPort::StopBits stopBits, QSerialPort::FlowControl flowControl);

    void resetToDefault();
};
