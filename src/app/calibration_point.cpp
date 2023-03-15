#include "calibration_point.h"

CalibrationPoint::CalibrationPoint()
    : x{ 0.0 }
    , y{ 0.0 }
    , z{ 0.0 }
{ }

CalibrationPoint::CalibrationPoint(double _x, double _y, double _z)
    : x{ _x }
    , y{ _y }
    , z{ _z }
{ }

Vector10d CalibrationPoint::toVect() const
{
    Vector10d v;
    v[0] = x*x;
    v[1] = y*y;
    v[2] = z*z;
    v[3] = 2.0 * y * z;
    v[4] = 2.0 * x * z;
    v[5] = 2.0 * x * y;
    v[6] = 2.0 * x;
    v[7] = 2.0 * y;
    v[8] = 2.0 * z;
    v[9] = 1.0;
    return v;
}

const double& CalibrationPoint::getX() const
{
    return x;
}

const double& CalibrationPoint::getY() const
{
    return y;
}

const double& CalibrationPoint::getZ() const
{
    return z;
}

std::ostream& operator<<(std::ostream& os, const CalibrationPoint& cp)
{
    os << "x: " << cp.x << ", y: " << cp.y << ", z: " << cp.z;
    return os;
}

