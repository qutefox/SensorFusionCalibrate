#include "calibrate.h"

const Eigen::Matrix<double, 6, 6> Calibrate::preInvertedConstraintMatrix
{
    {0.0, 0.5, 0.5, 0.0, 0.0, 0.0},
    {0.5, 0.0, 0.5, 0.0, 0.0, 0.0},
    {0.5, 0.5, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, -0.25, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0, -0.25, 0.0},
    {0.0, 0.0, 0.0, 0.0, 0.0, -0.25},
};

Calibrate::Calibrate(QObject* parent)
    : QObject{ parent }
{

    addPoint(1, 1, 1);
    addPoint(2, 2, 2);
    addPoint(3, 3, 3);
    addPoint(1, 2, 3);
    addPoint(3, 2, 1);
    addPoint(6, 7, 8);
    addPoint(4, 5, 6);
    addPoint(1, 1, 1);
    addPoint(1, 2, 4);
    addPoint(1, 3, 8);
    addPoint(4, 5, 4);
    calibrate();
}

void Calibrate::addPoint(double x, double y, double z)
{
    points.push_back({x, y, z});
    // calibrate(); // TODO: guard 'points' vector change during calibration.
}

void Calibrate::reset()
{
    points.clear();
}

void Calibrate::calibrate()
{
    Eigen::Matrix<double, 10, Eigen::Dynamic> inputMatrix;
    inputMatrix.resize(Eigen::NoChange, points.size());
    for (std::size_t i = 0 ; i < points.size() ; ++i)
    {
        inputMatrix.col(i) = points[i].toVect();
    }

    Matrix10x10d symmetricInputMatrix = inputMatrix * inputMatrix.transpose();
    Matrix6x6d symmetric11   = symmetricInputMatrix.block(0, 0, 6, 6);
    Matrix6x4d symmetric12   = symmetricInputMatrix.block(0, 6, 6, 4);
    Matrix4x6d symmetric12t  = symmetricInputMatrix.block(6, 0, 4, 6);
    Matrix4x4d symmetric22   = symmetricInputMatrix.block(6, 6, 4, 4);
    Matrix4x4d symmetric22_1 = symmetric22.llt().solve(Matrix4x4d::Identity());
    Matrix4x6d symmetric22a = symmetric22_1 * symmetric12t;
    Matrix6x6d symmetric22b = symmetric12 * symmetric22a;
    Matrix6x6d ss = symmetric11 - symmetric22b;
    Matrix6x6d e = preInvertedConstraintMatrix * ss;

    Eigen::EigenSolver<Matrix6x6d> eigenSolver(e, true);
    Eigen::Index maxIndex;
    eigenSolver.eigenvalues().real().maxCoeff(&maxIndex);
    Eigen::VectorXcd eigenVector = eigenSolver.eigenvectors().col(maxIndex);
    Eigen::VectorXd v1 = eigenVector.real();
    Vector4d v2 = symmetric22a * v1;
    Vector10d v {v1[0], v1[1], v1[2], v1[3], v1[4], v1[5], -v2[0], -v2[1], -v2[2], -v2[3]};
    Matrix3x3d q {{v[0], v[5], v[4]}, {v[5], v[1], v[3]}, {v[4], v[3], v[2]}};
    Vector3d u {v[6], v[7], v[8]};
    Matrix3x3d q1 = q.llt().solve(Matrix3x3d::Identity());

    bias = q1 * u;
    bias *= -1;
    // b[0] -> x-axis combined bias
    // b[1] -> y-axis combined bias
    // b[2] -> z-axis combined bias
    Vector3d qb = q * bias;
    Vector1d btqb = bias.transpose() * qb;
    double hmb = std::sqrt(btqb(0,0)-v[9]);

    Eigen::EigenSolver<Matrix3x3d> eigenSolver2(q, true);
    Vector3d eigenReal = eigenSolver2.eigenvalues().real();

    Matrix3x3d dz = Eigen::MatrixXd::Zero(3,3);
    dz(0, 0) = std::sqrt(eigenReal[0]);
    dz(1, 1) = std::sqrt(eigenReal[1]);
    dz(2, 2) = std::sqrt(eigenReal[2]);

    Matrix3x3d vdz = eigenSolver2.eigenvectors().real() * dz;
    Matrix3x3d sq = vdz * eigenSolver2.eigenvectors().real().transpose();
    double hm = 0.569; // TODO: config?!
    tensor = sq * hm / hmb;
}
