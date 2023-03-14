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

Calibrate::Calibrate(QObject* parent)
    : QObject{ parent }
{
    std::cout << preInvertedConstraintMatrix << "\n";

    AddPoint(1,1,1);
    AddPoint(2, 2, 2);
    AddPoint(3, 3, 3);
}

void Calibrate::AddPoint(double x, double y, double z)
{
    Vector10d item = xyzToVect10d(x, y, z);
    input.conservativeResize(input.rows(), input.cols()+1);
    input.col(input.cols()-1) = item;
    std::cout << input << "\n----------\n";
}

Vector10d Calibrate::xyzToVect10d(double x, double y, double z)
{
    Vector10d v;
    v[0] = x*x;
    v[1] = y*y;
    v[2] = z*z;
    v[3] = 2.0 * y * z;
    v[4] = 2.0 * x * z;
    v[5] = 2.0 * x * y;
    v[6] = 2.0 * x;
    v[7] = 2.0 * y;
    v[8] = 2.0 * z;
    v[9] = 1.0;
    return v;
}
