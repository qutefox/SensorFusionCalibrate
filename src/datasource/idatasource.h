#pragma once

#include <set>

#include "../data/point.h"

class IDataSource
{
public:
    virtual ~IDataSource() {}
    virtual std::set<Point>&& getNextPoints(long long maxNumberOfPoints = -1) = 0;
    virtual long long totalNumberOfPoints() = 0;

};
