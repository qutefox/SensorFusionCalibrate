#include "point.h"

Point::Point()
    : m_x{ 0.0 }
    , m_y{ 0.0 }
    , m_z{ 0.0 }
{ }

Point::Point(double x, double y, double z)
    : m_x{ x }
    , m_y{ y }
    , m_z{ z }
{ }

Vector10d Point::toVect() const
{
    Vector10d v;
    v[0] = m_x*m_x;
    v[1] = m_y*m_y;
    v[2] = m_z*m_z;
    v[3] = 2.0 * m_y * m_z;
    v[4] = 2.0 * m_x * m_z;
    v[5] = 2.0 * m_x * m_y;
    v[6] = 2.0 * m_x;
    v[7] = 2.0 * m_y;
    v[8] = 2.0 * m_z;
    v[9] = 1.0;
    return v;
}

const double& Point::getX() const
{
    return m_x;
}

const double& Point::getY() const
{
    return m_y;
}

const double& Point::getZ() const
{
    return m_z;
}

bool Point::operator<(const Point& rhs) const
{
    if (m_x != rhs.m_x ) return m_x < rhs.m_x;
    if (m_y != rhs.m_y ) return m_y < rhs.m_y;
    return m_z < rhs.m_z;
}

bool Point::operator==(const Point& rhs) const
{
    return m_x == rhs.m_x && m_y == rhs.m_y && m_z == rhs.m_z;
}

bool Point::operator!=(const Point& rhs) const
{
    return !operator==(rhs);
}

std::ostream& operator<<(std::ostream& os, const Point& cp)
{
    os << "x: " << cp.getX() << ", y: " << cp.getY() << ", z: " << cp.getZ();
    return os;
}

