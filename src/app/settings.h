#pragma once

#include <memory>

#include <QSettings>

enum DataSourceType
{
    CSV_FILE = 0,
    SERIAL_PORT = 1
};

class Settings
{

public:
    Settings();
    virtual ~Settings();

    DataSourceType getDataSourceType() const;
    void setDataSourceType(DataSourceType t);

private:
    std::unique_ptr<QSettings> m_settings;
};
