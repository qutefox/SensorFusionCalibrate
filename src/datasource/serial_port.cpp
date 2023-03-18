#include "serial_port.h"

SerialPortDataSource::SerialPortDataSource()
{

}

SerialPortDataSource::~SerialPortDataSource()
{
    m_serial.close();
}

std::set<Point>&& SerialPortDataSource::getNextPoints()
{
    return std::move(m_points);
}

bool SerialPortDataSource::applyConfig(const QMap<QString, QVariant>& config)
{
    return false;
}
