#include "settings.h"

Settings::Settings()
{
    m_settings = std::make_unique<QSettings>(QSettings::SystemScope, "SensorFusionCalibrate");
}

Settings::~Settings()
{
    m_settings.reset();
}

DataSourceType Settings::getDataSourceType() const
{
    QString tmp = m_settings->value("dataSourceType").toString();
    if (tmp == "serial") return DataSourceType::SERIAL_PORT;
    return DataSourceType::CSV_FILE;
}

void Settings::setDataSourceType(DataSourceType t)
{
    QString tmp;
    switch (t)
    {
    case DataSourceType::CSV_FILE:
        tmp = "csv_file";
        break;
    case DataSourceType::SERIAL_PORT:
        tmp = "serial";
        break;
    }
    if (tmp.isEmpty()) return;
    m_settings->setValue("dataSourceType", tmp);
}
