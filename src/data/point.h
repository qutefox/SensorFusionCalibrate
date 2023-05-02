#pragma once

#include <iostream>
#include <Eigen/Dense>

#include <data/data_types.h>

class Point
{
public:
    Point();
    Point(double x, double y, double z);

    Vector10d toVect() const;

    const double& getX() const;
    const double& getY() const;
    const double& getZ() const;

    bool operator<(const Point& rhs) const;
    bool operator==(const Point& rhs) const;
    bool operator!=(const Point& rhs) const;

    friend std::ostream& operator<<(std::ostream& os, const Point& cp);

private:
    double m_x = 0.0;
    double m_y = 0.0;
    double m_z = 0.0;
};

std::ostream& operator<<(std::ostream& os, const Point& cp);
