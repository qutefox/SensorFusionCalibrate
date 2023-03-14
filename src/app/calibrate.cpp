#include "calibrate.h"
#include <iostream>

const Eigen::Matrix<double, 6, 6> Calibrate::preInvertedConstraintMatrix
{
    {0.0, 0.5, 0.5, 0.0, 0.0, 0.0},
    {0.5, 0.0, 0.5, 0.0, 0.0, 0.0},
    {0.5, 0.5, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, -0.25, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0, -0.25, 0.0},
    {0.0, 0.0, 0.0, 0.0, 0.0, -0.25},
};

/*
void print_matrix(double *X, int m, int n) {
    int i,j;
    int idx = 0;
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
        {
            printf("[%d] %8.2f ",idx++,*X++);
        }
        printf("\n");
    }
}
*/

Calibrate::Calibrate(QObject* parent)
    : QObject{ parent }
{
    // std::cout << preInvertedConstraintMatrix << "\n";


}

void Calibrate::addPoint(double x, double y, double z)
{
    points.push_back({x, y, z});
    calibrate();
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

    Eigen::Matrix<double, 10, 10> symmetricInputMatrix = inputMatrix.transpose().lazyProduct(inputMatrix);

    // Eigen::Matrix<double, 6, 6> s11;


    // TODO: calculate tensors.
}
