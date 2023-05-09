#pragma once

#include <iostream>
#include <Eigen/Dense>

#include <data/data_types.h>

class Quaternion
{
public:
    Quaternion();
    Quaternion(double q0, double q1, double q2, double q3);

    Matrix3x3d toRotation() const;

    const double& getQ0() const;
    const double& getW() const;

    const double& getQ1() const;
    const double& getX() const;

    const double& getQ2() const;
    const double& getY() const;

    const double& getQ3() const;
    const double& getZ() const;

    bool operator==(const Quaternion& rhs) const;
    bool operator!=(const Quaternion& rhs) const;

    friend std::ostream& operator<<(std::ostream& os, const Quaternion& cp);

private:
    double m_q0 = 0.0; // w
    double m_q1 = 0.0; // x
    double m_q2 = 0.0; // y
    double m_q3 = 0.0; // z
};

std::ostream& operator<<(std::ostream& os, const Quaternion& cp);
