#include "quaternion.h"

Quaternion::Quaternion()
    : m_q0{ 0.0 }
    , m_q1{ 0.0 }
    , m_q2{ 0.0 }
    , m_q3{ 0.0 }
{ }

Quaternion::Quaternion(double q0, double q1, double q2, double q3)
    : m_q0{ q0 }
    , m_q1{ q1 }
    , m_q2{ q2 }
    , m_q3{ q3 }
{ }

Matrix3x3d Quaternion::toRotation() const
{
    Matrix3x3d rmatrix;
    const double& qw = m_q0;
	const double& qx = m_q1;
	const double& qy = m_q2;
	const double& qz = m_q3;
	rmatrix(0, 0) = 1.0f - 2.0f * qy * qy - 2.0f * qz * qz;
	rmatrix(0, 1) = 2.0f * qx * qy - 2.0f * qz * qw;
	rmatrix(0, 2) = 2.0f * qx * qz + 2.0f * qy * qw;
	rmatrix(1, 0) = 2.0f * qx * qy + 2.0f * qz * qw;
	rmatrix(1, 1) = 1.0f  - 2.0f * qx * qx - 2.0f * qz * qz;
	rmatrix(1, 2) = 2.0f * qy * qz - 2.0f * qx * qw;
	rmatrix(2, 0) = 2.0f * qx * qz - 2.0f * qy * qw;
	rmatrix(2, 1) = 2.0f * qy * qz + 2.0f * qx * qw;
	rmatrix(2, 2) = 1.0f  - 2.0f * qx * qx - 2.0f * qy * qy;
    return rmatrix;
}

const double& Quaternion::getQ0() const
{
    return m_q0;
}

const double& Quaternion::getW() const
{
    return m_q0;
}

const double& Quaternion::getQ1() const
{
    return m_q1;
}

const double& Quaternion::getX() const
{
    return m_q1;
}

const double& Quaternion::getQ2() const
{
    return m_q2;
}

const double& Quaternion::getY() const
{
    return m_q2;
}

const double& Quaternion::getQ3() const
{
    return m_q3;
}

const double& Quaternion::getZ() const
{
    return m_q3;
}

bool Quaternion::operator==(const Quaternion& rhs) const
{
    return m_q0 == rhs.m_q0 && m_q1 == rhs.m_q1 && m_q2 == rhs.m_q2 && m_q3 == rhs.m_q3;
}

bool Quaternion::operator!=(const Quaternion& rhs) const
{
    return !operator==(rhs);
}

std::ostream& operator<<(std::ostream& os, const Quaternion& cp)
{
    os << "w: " << cp.getW() << ", x: " << cp.getX() << ", y: " << cp.getY() << ", z: " << cp.getZ();
    return os;
}
