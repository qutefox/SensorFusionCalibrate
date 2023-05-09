#pragma once

#include <iostream>
#include <Eigen/Dense>

#include <data/data_types.h>
#include <data/calibration_result.h>

class Point
{
public:
    Point();
    Point(double x, double y, double z);

    Vector10d toVect() const;
    Point rotate(const Matrix3x3d& rMatrix) const;
    Point applyCalibration(const CalibrationResult& cr);

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
