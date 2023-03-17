#pragma once

#include <set>

#include <QSerialPort>

#include "idatasource.h"

class SerialPortDataSource : IDataSource
{

public:
    SerialPortDataSource();
    virtual ~SerialPortDataSource();

    std::set<Point>&& getNextPoints(long long maxNumberOfPoints = -1) override;
    long long totalNumberOfPoints() override;

private:
    QSerialPort m_serial;
    std::set<Point> m_points;
};
