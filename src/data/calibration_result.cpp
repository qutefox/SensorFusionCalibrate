#include "calibration_result.h"

CalibrationResult::CalibrationResult()
{

}

void CalibrationResult::reset()
{
    biasVector.setZero();
    transformationMatrix.setZero();
}
