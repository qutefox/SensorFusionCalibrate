#include "settings.h"

#include <QStandardPaths>

Settings::Settings()
{
    QString configPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    QString iniFileName = "settings.ini";
    m_settings = std::make_unique<QSettings>(configPath+"/"+iniFileName, QSettings::IniFormat);
}

Settings::~Settings()
{
    m_settings.reset();
}

QString Settings::getDataSourceType() const
{
    QString tmp = m_settings->value("dataSourceType", QVariant("")).toString();
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

CSVFileConfig Settings::getCSVFileConfig() const
{
    CSVFileConfig config;
    config.m_filePath = m_settings->value("csvFilePath", QVariant(config.m_filePath)).toString();
    config.m_browseDirectoryPath = m_settings->value("csvFileBrowseDirectoryPath", QVariant(config.m_browseDirectoryPath)).toString();
    return config;
}

void Settings::setCSVFileConfig(const CSVFileConfig& config)
{
    m_settings->setValue("csvFilePath", config.m_filePath);
    m_settings->setValue("csvFileBrowseDirectoryPath", config.m_browseDirectoryPath);
}

SerialPortConfig Settings::getSerialPortConfig() const
{
    SerialPortConfig config;

    config.m_portName = m_settings->value("serialPortName", QVariant("")).toString();
    config.m_autoConnectPortId = m_settings->value("serialPortAutoConnectPortName", QVariant("")).toString();
    config.m_baudRate = m_settings->value("serialPortBaudRate", QVariant(config.m_baudRate)).toUInt();

    int tmp = m_settings->value("serialPortDataBits", QVariant(config.m_dataBits)).toInt();
    config.m_dataBits = QSerialPort::DataBits(tmp);

    tmp = m_settings->value("serialPortParity", QVariant(config.m_parity)).toInt();
    config.m_parity = QSerialPort::Parity(tmp);

    tmp = m_settings->value("serialPortStopBits", QVariant(config.m_stopBits)).toInt();
    config.m_stopBits = QSerialPort::StopBits(tmp);

    tmp = m_settings->value("serialPortFlowControl", QVariant(config.m_flowControl)).toInt();
    config.m_flowControl =QSerialPort::FlowControl(tmp);

    return config;
}

void Settings::setSerialPortConfig(const SerialPortConfig& config)
{
    m_settings->setValue("serialPortName", config.m_portName);
    m_settings->setValue("serialPortAutoConnectPortName", config.m_autoConnectPortId);
    m_settings->setValue("serialPortBaudRate", config.m_baudRate);
    m_settings->setValue("serialPortDataBits", config.m_dataBits);
    m_settings->setValue("serialPortParity", config.m_parity);
    m_settings->setValue("serialPortStopBits", config.m_stopBits);
    m_settings->setValue("serialPortFlowControl", config.m_flowControl);
}
