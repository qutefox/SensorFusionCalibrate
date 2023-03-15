#include "calibrate.h"
#include <iostream>
#include <fstream>

#include "../math/matrices.h"
#include "../math/data_movement.h"
#include "../math/choleski.h"
#include "../math/multiply_matrices.h"
#include "../math/hessenberg_elementary.h"
#include "../math/qr_hessenberg_matrix.h"

const Eigen::Matrix<double, 6, 6> Calibrate::preInvertedConstraintMatrix
{
    {0.0, 0.5, 0.5, 0.0, 0.0, 0.0},
    {0.5, 0.0, 0.5, 0.0, 0.0, 0.0},
    {0.5, 0.5, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, -0.25, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0, -0.25, 0.0},
    {0.0, 0.0, 0.0, 0.0, 0.0, -0.25},
};


void print_matrix(double *X, int m, int n)
{
    int i,j;
    // int idx = 0;
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
        {
            // double tmp = *X;
            //printf(" %8.2f ",idx++,*X++);
            std::cout << "  " << *X++;
        }
        std::cout << "\r\n";
    }
}

void Calibrate::test()
{
    int nlines = 0;
    char buf[120];
    double *D, *S, *S11, *S12, *S12t, *S22, *S22_1, *S22a, *S22b, *SS, *E, *C, *SSS;
    double *eigen_real, *eigen_imag, *v1, *v2, *v, *Q, *Q_1, *B, *QB, J, hmb, *SSSS;
    int *p;
    int i, index;
    double x, y, z;
    double maxval, norm, btqb, *eigen_real3, *eigen_imag3, *Dz, *vdz, *SQ, *A_1, hm, norm1, norm2, norm3;

    FILE *fp;
    fp = fopen("C:\\Users\\balaz\\OneDrive\\Documents\\SensorFusionCalibrate\\src\\test.txt", "r");

    while(fgets(buf, 100, fp) != NULL)
    {
        nlines++;
    }
    fclose(fp);

    std::string t_str;
    std::ifstream in("C:\\Users\\balaz\\OneDrive\\Documents\\SensorFusionCalibrate\\src\\test.txt");

    D = (double*)malloc(10 * nlines * sizeof(double));
    i = 0;
    while (std::getline(in, t_str))
    {
        std::istringstream in(t_str);
        in >> x >> y >> z;

        D[i] = x * x;
        D[nlines+i] = y * y;
        D[nlines*2+i] = z * z;
        D[nlines*3+i] = 2.0 * y * z;
        D[nlines*4+i] = 2.0 * x * z;
        D[nlines*5+i] = 2.0 * x * y;
        D[nlines*6+i] = 2.0 * x;
        D[nlines*7+i] = 2.0 * y;
        D[nlines*8+i] = 2.0 * z;
        D[nlines*9+i] = 1.0;

        ++i;
    }
    in.close();

    C = (double*)malloc(6 * 6 * sizeof(double));
    C[0] = 0.0; C[1] = 0.5; C[2] = 0.5; C[3] = 0.0;  C[4] = 0.0;  C[5] = 0.0;
    C[6] = 0.5;  C[7] = 0.0; C[8] = 0.5; C[9] = 0.0;  C[10] = 0.0;  C[11] = 0.0;
    C[12] = 0.5;  C[13] = 0.5; C[14] = 0.0; C[15] = 0.0;  C[16] = 0.0;  C[17] = 0.0;
    C[18] = 0.0;  C[19] = 0.0;  C[20] = 0.0;  C[21] = -0.25; C[22] = 0.0;  C[23] = 0.0;
    C[24] = 0.0;  C[25] = 0.0; C[26] = 0.0;  C[27] = 0.0;  C[28] = -0.25; C[29] = 0.0;
    C[30] = 0.0;  C[31] = 0.0; C[32] = 0.0;  C[33] = 0.0;  C[34] = 0.0;  C[35] = -0.25;

    // std::cout << "test D:\r\n";
    // print_matrix(D, 10, nlines);

    S = (double*)malloc(10 * 10 * sizeof(double));
    Matrix_x_Its_Transpose(S, D, 10, nlines);

    // std::cout << "test S:\r\n";
    // print_matrix(S, 10, nlines);
    // std::cout << "\r\n";

    S11 = (double*)malloc(6 * 6 * sizeof(double));
    Get_Submatrix(S11, 6, 6, S, 10, 0, 0);

    // std::cout << "test S11:\r\n";
    // print_matrix(S11, 6, 6);
    // std::cout << "\r\n";

    S12 = (double*)malloc(6 * 4 * sizeof(double));
    Get_Submatrix(S12, 6, 4, S, 10, 0, 6);

    // std::cout << "test S12:\r\n";
    // print_matrix(S12, 6, 4);
    // std::cout << "\r\n";

    S12t = (double*)malloc(4 * 6 * sizeof(double));
    Get_Submatrix(S12t, 4, 6, S, 10, 6, 0);

    // std::cout << "test S12t:\r\n";
    // print_matrix(S12t, 4, 6);
    // std::cout << "\r\n";

    S22 = (double*)malloc(4 * 4 * sizeof(double));
    Get_Submatrix(S22, 4, 4, S, 10, 6, 6);

    // std::cout << "test S22:\r\n";
    // print_matrix(S22, 4, 4);
    // std::cout << "\r\n";

    S22_1 = (double*)malloc(4 * 4 * sizeof(double));
    for(i = 0; i < 16; i++)
    {
        S22_1[i] = S22[i];
    }

    Choleski_LU_Decomposition(S22_1, 4);
    Choleski_LU_Inverse(S22_1, 4);

    // std::cout << "test S22_1:\r\n";
    // print_matrix(S22_1, 4, 4);
    // std::cout << "\r\n";

    // Calculate S22a = S22_1 * S12t   4*6 = 4x4 * 4x6   C = AB
    S22a = (double*)malloc(4 * 6 * sizeof(double));
    Multiply_Matrices(S22a, S22_1, 4, 4, S12t, 6);

    // std::cout << "test S22a:\r\n";
    // print_matrix(S22a, 4, 6);
    // std::cout << "\r\n";

    // Then calculate S22b = S12 * S22a      ( 6x6 = 6x4 * 4x6)
    S22b = (double*)malloc(6 * 6 * sizeof(double));
    Multiply_Matrices(S22b, S12, 6, 4, S22a, 6);

    // std::cout << "test S22b:\r\n";
    // print_matrix(S22b, 6, 6);
    // std::cout << "\r\n";

    // Calculate SS = S11 - S22b
    SS = (double*)malloc(6 * 6 * sizeof(double));
    for(i = 0; i < 36; i++)
    {
        SS[i] = S11[i] - S22b[i];
    }

    // std::cout << "test SS:\r\n";
    // print_matrix(SS, 6, 6);
    // std::cout << "\r\n";

    E = (double*)malloc(6 * 6 * sizeof(double));
    Multiply_Matrices(E, C, 6, 6, SS, 6);

    // std::cout << "test E:\r\n";
    // print_matrix(E, 6, 6);
    // std::cout << "\r\n";

    SSS = (double*)malloc(6 * 6 * sizeof(double));
    Hessenberg_Form_Elementary(E, SSS, 6);

    std::cout << "test SSS0:\r\n";
    print_matrix(SSS, 6, 6);
    std::cout << "\r\n";

    eigen_real = (double*)malloc(6 * sizeof(double));
    eigen_imag = (double*)malloc(6 * sizeof(double));

    QR_Hessenberg_Matrix(E, SSS, eigen_real, eigen_imag, 6, 100);

    std::cout << "test SSS1:\r\n";
    print_matrix(SSS, 6, 6);
    std::cout << "\r\n";

    // std::cout << "test eigen_real:\r\n";
    // print_matrix(eigen_real, 1, 6);
    // std::cout << "\r\n";

    // std::cout << "test eigen_imag:\r\n";
    // print_matrix(eigen_imag, 1, 6);
    // std::cout << "\r\n";

    index = 0;
    maxval = eigen_real[0];
    for(i = 1; i < 6; i++)
    {
        if(eigen_real[i] > maxval)
        {
            maxval = eigen_real[i];
            index = i;
        }
    }
    // std::cout << "maxval index: " << index << "\r\n";

    v1 = (double*)malloc(6 * sizeof(double));
    v1[0] = SSS[index];
    v1[1] = SSS[index+6];
    v1[2] = SSS[index+12];
    v1[3] = SSS[index+18];
    v1[4] = SSS[index+24];
    v1[5] = SSS[index+30];

    // std::cout << "test v1:\r\n";
    // print_matrix(v1, 1, 6);
    // std::cout << "\r\n";

    fflush(stdout);

    free(C);
    free(D);
    free(S);
    free(S11);
    free(S12);
    free(S12t);
    free(S22);
    free(S22_1);
    free(S22a);
    free(S22b);
    free(SS);
    free(E);
    free(SSS);
    free(eigen_real);
    free(eigen_imag);
}

Calibrate::Calibrate(QObject* parent)
    : QObject{ parent }
{
    // std::cout << preInvertedConstraintMatrix << "\n";

    test();

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
    calibrate();

}

void Calibrate::addPoint(double x, double y, double z)
{
    points.push_back({x, y, z});
    // calibrate();
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

    Eigen::Matrix<double, 10, 10> symmetricInputMatrix = inputMatrix * inputMatrix.transpose();
    Eigen::Matrix<double, 6, 6> symmetric11   = symmetricInputMatrix.block(0, 0, 6, 6);
    Eigen::Matrix<double, 6, 4> symmetric12   = symmetricInputMatrix.block(0, 6, 6, 4);
    Eigen::Matrix<double, 4, 6> symmetric12t  = symmetricInputMatrix.block(6, 0, 4, 6);
    Eigen::Matrix<double, 4, 4> symmetric22   = symmetricInputMatrix.block(6, 6, 4, 4);
    Eigen::Matrix<double, 4, 4> symmetric22_1 = symmetric22.llt().solve(Eigen::Matrix<double, 4, 4>::Identity());
    Eigen::Matrix<double, 4, 6> symmetric22a  = symmetric22_1 * symmetric12t;
    Eigen::Matrix<double, 6, 6> symmetric22b  = symmetric12 * symmetric22a;
    Eigen::Matrix<double, 6, 6> ss            = symmetric11 - symmetric22b;
    Eigen::Matrix<double, 6, 6> e             = preInvertedConstraintMatrix * ss;



    Eigen::HessenbergDecomposition<Eigen::Matrix<double, 6, 6>> hess;
    //Eigen::Matrix<double, 6, 6> hessDec = hess.compute(e).matrixH();
    std::cout << "hess:\r\n" << hess.compute(e).householderCoefficients() << "\r\n";
    // std::cout << "hess:\r\n" << hess.compute(e).matrixQ() << "\r\n";
    std::cout << "hess:\r\n" << e.householderQr().matrixQR() << "\r\n";


    // Eigen::Matrix<std::complex<double>, 6, 1> eigenVal = e.eigenvalues();
    // std::cout << "eig_real:\r\n" << eigenVal.real() << "\r\n";
    // std::cout << "eig_imag:\r\n" << eigenVal.imag() << "\r\n";

    // std::cout << "q:\r\n" << sss << "\r\n";


    // TODO: calculate tensors.

    fflush(stdout);
}
