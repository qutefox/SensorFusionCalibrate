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

Point Point::rotate(const Matrix3x3d& rMatrix) const
{
    Point out;

    out.m_x = m_x * rMatrix(0, 0) + m_y * rMatrix(0, 1) + m_z * rMatrix(0, 2);
	out.m_y = m_x * rMatrix(1, 0) + m_y * rMatrix(1, 1) + m_z * rMatrix(1, 2);
	out.m_z = m_x * rMatrix(2, 0) + m_y * rMatrix(2, 1) + m_z * rMatrix(2, 2);

    return out;
}

Point Point::applyCalibration(const CalibrationResult& cr)
{
    const Vector3d& bias = cr.biasVector;
    const Matrix3x3d& trMatrix = cr.transformationMatrix;

    Point out;

	out.m_x = m_x - bias[0];
	out.m_y = m_y - bias[1];
	out.m_z = m_z - bias[2];

	out.m_x = out.m_x * trMatrix(0, 0) + out.m_y * trMatrix(0, 1) + out.m_z * trMatrix(0, 2);
	out.m_y = out.m_x * trMatrix(1, 0) + out.m_y * trMatrix(1, 1) + out.m_z * trMatrix(1, 2);
	out.m_z = out.m_x * trMatrix(2, 0) + out.m_y * trMatrix(2, 1) + out.m_z * trMatrix(2, 2);

    return out;
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

