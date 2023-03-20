#include "settings.h"

Settings::Settings()
{
    m_settings = std::make_unique<QSettings>(QSettings::SystemScope, "SensorFusionCalibrate");
}

Settings::~Settings()
{
    m_settings.reset();
}

QString Settings::getDataSourceType() const
{
    QString tmp = m_settings->value("dataSourceType").toString();
    if (tmp == "csv_file" || tmp == "serial") return tmp;
    return "";
}

void Settings::setDataSourceType(const QString& dataSourceType)
{
    if (dataSourceType == "csv_file" || dataSourceType == "serial")
    {
        m_settings->setValue("dataSourceType", dataSourceType);
    }
    else m_settings->setValue("dataSourceType", "");
}

QString Settings::getCSVFilePath() const
{
    return m_settings->value("csvFilePath").toString();
}

void Settings::setCSVFilePath(const QString& filePath)
{
    m_settings->setValue("csvFilePath", filePath);
}

SerialPortConfig Settings::getSerialPortConfig() const
{
    SerialPortConfig config;
    int tmp;
    config.m_portName = m_settings->value("serialPortName").toString();
    config.m_autoConnectPortName = m_settings->value("serialPortAutoConnectPortName").toString();
    config.m_baudRate = m_settings->value("serialPortBaudRate").toUInt();

    tmp = m_settings->value("serialPortDataBits").toInt();
    config.m_dataBits = QSerialPort::DataBits(tmp);

    tmp = m_settings->value("serialPortParity").toInt();
    config.m_parity = QSerialPort::Parity(tmp);

    tmp = m_settings->value("serialPortStopBits").toInt();
    config.m_stopBits = QSerialPort::StopBits(tmp);

    tmp = m_settings->value("serialPortFlowControl").toInt();
    config.m_flowControl =QSerialPort::FlowControl(tmp);

    return config;
}

void Settings::setSerialPortConfig(const SerialPortConfig& config)
{
    m_settings->setValue("serialPortName", config.m_portName);
    m_settings->setValue("serialPortAutoConnectPortName", config.m_autoConnectPortName);
    m_settings->setValue("serialPortBaudRate", config.m_baudRate);
    m_settings->setValue("serialPortDataBits", config.m_dataBits);
    m_settings->setValue("serialPortParity", config.m_parity);
    m_settings->setValue("serialPortStopBits", config.m_stopBits);
    m_settings->setValue("serialPortFlowControl", config.m_flowControl);
}
