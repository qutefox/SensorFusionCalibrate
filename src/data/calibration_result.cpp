#include "calibration_result.h"

CalibrationResult::CalibrationResult()
{

}

void CalibrationResult::reset()
{
    biasVector.setZero();
    transformationMatrix.setZero();
}

std::ostream& operator<<(std::ostream& os, const CalibrationResult& cr)
{
    const Vector3d& bias = cr.biasVector;
    const Matrix3x3d& trMatrix = cr.transformationMatrix;
    os << "bias:" << std::endl;
    os << "\tx=" << bias[0] << ", y=" << bias[1] << ", z=" << bias[2] << std::endl;
    os << "transformation matrix:" << std::endl;
    os << "\t11=" << trMatrix(0,0) << ", 12=" << trMatrix(0,1) << ", 13=" << trMatrix(1,2) << std::endl;
    os << "\t21=" << trMatrix(1,0) << ", 22=" << trMatrix(1,1) << ", 23=" << trMatrix(1,2) << std::endl;
    os << "\t31=" << trMatrix(2,0) << ", 32=" << trMatrix(2,1) << ", 33=" << trMatrix(2,2) << std::endl;
    return os;
}
