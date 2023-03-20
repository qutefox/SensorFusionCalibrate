#include "serialport_config.h"

SerialPortConfig::SerialPortConfig()
    : m_portName{ "" }
    , m_autoConnectPortName{ "" }
    , m_baudRate{ 115200 }
    , m_dataBits{ QSerialPort::DataBits::Data8 }
    , m_parity{ QSerialPort::Parity::NoParity }
    , m_stopBits{ QSerialPort::StopBits::OneStop }
    , m_flowControl{ QSerialPort::FlowControl::NoFlowControl }
{

}

SerialPortConfig::SerialPortConfig(const QString& portName, const QString& autoConnectPortName,
    unsigned int baudRate, QSerialPort::DataBits dataBits, QSerialPort::Parity parity,
    QSerialPort::StopBits stopBits, QSerialPort::FlowControl flowControl)
    : m_portName{ portName }
    , m_autoConnectPortName{ autoConnectPortName }
    , m_baudRate{ baudRate }
    , m_dataBits{ dataBits }
    , m_parity{ parity }
    , m_stopBits{ stopBits }
    , m_flowControl{ flowControl }
{

}

void SerialPortConfig::resetToDefault()
{
    m_portName.clear();
    m_autoConnectPortName.clear();
    m_baudRate = 115200;
    m_dataBits = QSerialPort::DataBits::Data8;
    m_parity = QSerialPort::Parity::NoParity;
    m_stopBits = QSerialPort::StopBits::OneStop;
    m_flowControl = QSerialPort::FlowControl::NoFlowControl;
}
