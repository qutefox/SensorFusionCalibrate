#pragma once

#include <iostream>

#include "data_types.h"

class CalibrationResult
{
public:
    Vector3d biasVector;
    Matrix3x3d transformationMatrix;

    CalibrationResult();

    void reset();

    friend std::ostream& operator<<(std::ostream& os, const CalibrationResult& cr);
};

std::ostream& operator<<(std::ostream& os, const CalibrationResult& cr);
