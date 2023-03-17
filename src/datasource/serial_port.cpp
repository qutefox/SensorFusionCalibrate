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

long long SerialPortDataSource::totalNumberOfPoints()
{
    return -1; // It's a stream. We can't know.
}
