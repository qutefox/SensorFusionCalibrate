#include "matrices.h"

void Matrix_x_a_Matrix_Transposed(double* C, double* A, int nrows, int ncols, double* B, int mrows)
{
    int i,j,k;
    double *pA;
    double *pB;
    for (i = 0; i < nrows; A += ncols, i++)
    {
        for (pB = B, j = 0; j < mrows; C++, j++)
        {
            for (pA = A, *C = 0.0, k  = 0; k < ncols; k++)
            {
                *C += *pA++ * *pB++;
            }
        }
    }
}

void Matrix_x_Its_Transpose(double *C, double *A, int nrows, int ncols)
{
    int i,j,k;
    double *pAi0 = A;
    double *pAj0;
    double *pCi0 = C;
    double *pCji;

    for (i = 0; i < nrows; pCi0 += nrows, pAi0 += ncols, i++)
    {
        pCji = pCi0 + i;
        pAj0 = pAi0;
        for (j = i; j < nrows; pCji += nrows, j++)
        {
            *(pCi0 + j) = 0.0;
            for (k = 0; k < ncols; k++)
            {
                *(pCi0 + j) += *(pAi0 + k) * *pAj0++;
            }
            *pCji = *(pCi0 + j);
        }
    }
}

void Matrix_Transposed_x_a_Matrix(double *C, double *A, int nrows, int ncols, double *B, int mcols)
{
    int i,j,k;
    double *pA = A;
    double *pB;
    double *p_B;

    for (i = 0; i < ncols; A++, i++)
    {
        for (p_B = B, j = 0; j < mcols; C++, p_B++, j++)
        {
            pB = p_B;
            pA = A;
            *C = 0.0;
            for (k = 0; k < nrows; pA += ncols, pB += mcols, k++)
            {
                *C += *pA * *pB;
            }
        }
    }
}

void Matrix_Transposed_x_the_Matrix(double*C, double *A, int nrows, int ncols)
{
    double *pAki;
    double *pAkj;
    double *pA0i;
    double *pCij;
    double *pCji;
    double *pCii = C;
    int i,j,k;

    for (i = 0; i < ncols; pCii += ncols + 1, i++)
    {
        pCji = pCii;
        pCij = pCii;
        pA0i = A + i;
        for (j = i; j < ncols; pCij++, pCji += ncols, j++)
        {
            pAki = pA0i;
            pAkj = A + j;
            *pCij = 0.0;
            for (k = 0; k < nrows; pAki += ncols, pAkj += ncols, k++)
            {
                *pCij += *pAki * *pAkj;
            }
            *pCji = *pCij;
        }
    }
}

void CMatrix_x_a_CMatrix_Transposed(std::complex<double> *C, std::complex<double> *A, int nrows, int ncols, std::complex<double> *B, int mrows)
{
    int i,j,k;
    std::complex<double> *pA;
    std::complex<double> *pB;

    for (i = 0; i < nrows; A += ncols, i++)
    {
        for (pB = B, j = 0; j < mrows; C++, j++)
        {
            for (pA = A, *C = 0.0, k  = 0; k < ncols; k++) *C += *pA++ * *pB++;
        }
    }
}

void CMatrix_x_Its_Transpose(std::complex<double> *C, std::complex<double> *A, int nrows, int ncols)
{
    int i,j,k;
    std::complex<double> *pAi0 = A;
    std::complex<double> *pAj0;
    std::complex<double> *pCi0 = C;
    std::complex<double> *pCji;

    for (i = 0; i < nrows; pCi0 += nrows, pAi0 += ncols, i++)
    {
        pCji = pCi0 + i;
        pAj0 = pAi0;
        for (j = i; j < nrows; pCji += nrows, j++)
        {
            *(pCi0 + j) = 0.0;
            for (k = 0; k < ncols; k++)
            {
                *(pCi0 + j) += *(pAi0 + k) * *pAj0++;
            }
            *pCji = *(pCi0 + j);
        }
    }
}

void CMatrix_Transposed_x_a_CMatrix(std::complex<double> *C, std::complex<double> *A, int nrows, int ncols, std::complex<double> *B, int mcols)
{
    int i,j,k;
    std::complex<double> *pA = A;
    std::complex<double> *pB;
    std::complex<double> *p_B;

    for (i = 0; i < ncols; A++, i++)
    {
        for (p_B = B, j = 0; j < mcols; C++, p_B++, j++)
        {
            pB = p_B;
            pA = A;
            *C = 0.0;
            for (k = 0; k < nrows; pA += ncols, pB += mcols, k++)
            {
                *C += *pA * *pB;
            }
        }
    }
}

void CMatrix_Transposed_x_the_CMatrix(std::complex<double> *C, std::complex<double> *A, int nrows, int ncols)
{
    std::complex<double> *pAki;
    std::complex<double> *pAkj;
    std::complex<double> *pA0i;
    std::complex<double> *pCij;
    std::complex<double> *pCji;
    std::complex<double> *pCii = C;
    int i,j,k;

    for (i = 0; i < ncols; pCii += ncols + 1, i++)
    {
        pCji = pCii;
        pCij = pCii;
        pA0i = A + i;
        for (j = i; j < ncols; pCij++, pCji += ncols, j++)
        {
            pAki = pA0i;
            pAkj = A + j;
            *pCij = 0.0;
            for (k = 0; k < nrows; pAki += ncols, pAkj += ncols, k++)
            {
                *pCij += *pAki * *pAkj;
            }
            *pCji = *pCij;
        }
    }
}

void CMatrix_x_a_CMatrix_Daggered(std::complex<double> *C, std::complex<double> *A, int nrows, int ncols, std::complex<double> *B, int mrows)
{
    int i,j,k;
    std::complex<double> *pA;
    std::complex<double> *pB;

    for (i = 0; i < nrows; A += ncols, i++)
    {
        for (pB = B, j = 0; j < mrows; C++, j++)
        {
            for (pA = A, *C = 0.0, k  = 0; k < ncols; k++)
            {
                *C += *pA++ * conj(*pB++);
            }
        }
    }
}

void CMatrix_x_Its_Dagger(std::complex<double> *C, std::complex<double> *A, int nrows, int ncols)
{
    int i,j,k;
    std::complex<double> *pAi0 = A;
    std::complex<double> *pAj0;
    std::complex<double> *pCi0 = C;
    std::complex<double> *pCji;

    for (i = 0; i < nrows; pCi0 += nrows, pAi0 += ncols, i++)
    {
        pCji = pCi0 + i;
        pAj0 = pAi0;
        for (j = i; j < nrows; pCji += nrows, j++)
        {
            *(pCi0 + j) = 0.0;
            for (k = 0; k < ncols; k++)
            {
                *(pCi0 + j) += *(pAi0 + k) * conj(*pAj0++);
            }
            *pCji = conj(*(pCi0 + j));
        }
    }
}

void CMatrix_Daggered_x_a_CMatrix(std::complex<double> *C, std::complex<double> *A, int nrows, int ncols, std::complex<double> *B, int mcols)
{
   int i,j,k;
   std::complex<double> *pA = A;
   std::complex<double> *pB;
   std::complex<double> *p_B;

    for (i = 0; i < ncols; A++, i++)
    {
        for (p_B = B, j = 0; j < mcols; C++, p_B++, j++)
        {
            pB = p_B;
            pA = A;
            *C = 0.0;
            for (k = 0; k < nrows; pA += ncols, pB += mcols, k++)
            {
                *C += conj(*pA) * *pB;
            }
        }
    }
}

void CMatrix_Daggered_x_the_CMatrix(std::complex<double> *C, std::complex<double> *A, int nrows, int ncols)
{
    std::complex<double> *pAki;
    std::complex<double> *pAkj;
    std::complex<double> *pA0i;
    std::complex<double> *pCij;
    std::complex<double> *pCji;
    std::complex<double> *pCii = C;
    int i,j,k;

    for (i = 0; i < ncols; pCii += ncols + 1, i++)
    {
        pCji = pCii;
        pCij = pCii;
        pA0i = A + i;
        for (j = i; j < ncols; pCij++, pCji += ncols, j++)
        {
            pAki = pA0i;
            pAkj = A + j;
            *pCij = 0.0;
            for (k = 0; k < nrows; pAki += ncols, pAkj += ncols, k++)
            {
                *pCij += conj(*pAki) * *pAkj;
            }
            *pCji = conj(*pCij);
        }
    }
}

void CMatrix_Daggered_x_an_RMatrix(std::complex<double> *C, std::complex<double> *A, int nrows, int ncols, double *B, int mcols)
{
    int i,j,k;
    std::complex<double> *pA = A;
    double *pB;
    double *p_B;

    for (i = 0; i < ncols; A++, i++)
    {
        for (p_B = B, j = 0; j < mcols; C++, p_B++, j++)
        {
            pB = p_B;
            pA = A;
            *C = 0.0;
            for (k = 0; k < nrows; pA += ncols, pB += mcols, k++)
            {
                *C += conj(*pA) * *pB;
            }
        }
    }
}

void CMatrix_Transposed_x_an_RMatrix(std::complex<double> *C, std::complex<double> *A, int nrows, int ncols, double *B, int mcols)
{
    int i,j,k;
    std::complex<double> *pA = A;
    double *pB;
    double *p_B;

    for (i = 0; i < ncols; A++, i++)
    {
        for (p_B = B, j = 0; j < mcols; C++, p_B++, j++)
        {
            pB = p_B;
            pA = A;
            *C = 0.0;
            for (k = 0; k < nrows; pA += ncols, pB += mcols, k++)
            {
                *C += *pA * *pB;
            }
        }
    }
}

void CMatrix_x_an_RMatrix_Transposed(std::complex<double> *C, std::complex<double> *A, int nrows, int ncols, double *B, int mrows)
{
    int i,j,k;
    std::complex<double> *pA;
    double *pB;

    for (i = 0; i < nrows; A += ncols, i++)
    {
        for (pB = B, j = 0; j < mrows; C++, j++)
        {
            for (pA = A, *C = 0.0, k  = 0; k < ncols; k++)
            {
                *C += *pA++ * *pB++;
            }
        }
    }
}

void RMatrix_Transposed_x_a_CMatrix(std::complex<double> *C, double *A, int nrows, int ncols, std::complex<double> *B, int mcols)
{
    int i,j,k;
    double *pA = A;
    std::complex<double> *pB;
    std::complex<double> *p_B;

    for (i = 0; i < ncols; A++, i++)
    {
        for (p_B = B, j = 0; j < mcols; C++, p_B++, j++)
        {
            pB = p_B;
            pA = A;
            *C = 0.0;
            for (k = 0; k < nrows; pA += ncols, pB += mcols, k++)
            {
                *C += *pA * *pB;
            }
        }
    }
}

void RMatrix_x_a_CMatrix_Daggered(std::complex<double> *C, double *A, int nrows, int ncols, std::complex<double> *B, int mrows)
{
    int i,j,k;
    double *pA;
    std::complex<double> *pB;

    for (i = 0; i < nrows; A += ncols, i++)
    {
        for (pB = B, j = 0; j < mrows; C++, j++)
        {
            for (pA = A, *C = 0.0, k  = 0; k < ncols; k++)
            {
                *C += *pA++ * conj(*pB++);
            }
        }
    }
}

void RMatrix_x_a_CMatrix_Transposed(std::complex<double> *C, double *A, int nrows, int ncols, std::complex<double> *B, int mrows)
{
    int i,j,k;
    double *pA;
    std::complex<double> *pB;

    for (i = 0; i < nrows; A += ncols, i++)
    {
        for (pB = B, j = 0; j < mrows; C++, j++)
        {
            for (pA = A, *C = 0.0, k  = 0; k < ncols; k++)
            {
                *C += *pA++ * *pB++;
            }
        }
    }
}
