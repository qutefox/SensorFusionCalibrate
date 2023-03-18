#pragma once

#include "data_types.h"

class CalibrationResult
{
public:
    Vector3d biasVector;
    Matrix3x3d transformationMatrix;

    CalibrationResult();

    void reset();
};
