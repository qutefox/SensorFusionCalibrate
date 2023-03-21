#pragma once

#include <memory>

#include <QString>
#include <QSettings>

#include <data/serialport_config.h>
#include <data/csv_file_config.h>

class Settings
{

public:
    Settings();
    virtual ~Settings();

    QString getDataSourceType() const;
    void setDataSourceType(const QString& dataSourceType);

    CSVFileConfig getCSVFileConfig() const;
    void setCSVFileConfig(const CSVFileConfig& config);

    SerialPortConfig getSerialPortConfig() const;
    void setSerialPortConfig(const SerialPortConfig& config);

private:
    std::unique_ptr<QSettings> m_settings;
};
