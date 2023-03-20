#pragma once

#include <memory>

#include <QSettings>
#include <QString>
#include <data/serialport_config.h>

class Settings
{

public:
    Settings();
    virtual ~Settings();

    QString getDataSourceType() const;
    void setDataSourceType(const QString& dataSourceType);

    QString getCSVFilePath() const;
    void setCSVFilePath(const QString& filePath);

    SerialPortConfig getSerialPortConfig() const;
    void setSerialPortConfig(const SerialPortConfig& config);

private:
    std::unique_ptr<QSettings> m_settings;
};
