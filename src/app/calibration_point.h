#pragma once

#include <iostream>
#include <Eigen/Dense>

#include "calibrate_types.h"

class CalibrationPoint
{
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

public:
    CalibrationPoint();
    CalibrationPoint(double x, double y, double z);

    Vector10d toVect() const;

    const double& getX() const;
    const double& getY() const;
    const double& getZ() const;

    friend std::ostream& operator<<(std::ostream& os, const CalibrationPoint& cp);
};

std::ostream& operator<<(std::ostream& os, const CalibrationPoint& cp);
