#pragma once

#include <iostream>
#include <Eigen/Dense>

#include "calibrate_types.h"

class CalibrationPoint
{
public:
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    CalibrationPoint();
    CalibrationPoint(double _x, double _y, double _z);

    Vector10d toVect() const;

    friend std::ostream& operator<<(std::ostream& os, const CalibrationPoint& cp);
};

std::ostream& operator<<(std::ostream& os, const CalibrationPoint& cp);
