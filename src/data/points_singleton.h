#pragma once

#include <vector>
#include <set>
#include <memory>

#include <data/point.h>

class Points
{
    using PointSet = std::set<Point>;
public:
    Points();
    
    void addPoint(unsigned int id, const Point& p);

private:
    std::vector<std::shared_ptr<PointSet>> m_data;
};

extern Points g_Points;

