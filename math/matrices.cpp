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

void CMatrix_x_a_CMatrix_Transposed(double complex *C, double complex *A, int nrows, int ncols, double complex *B, int mrows)
{
    int i,j,k;
    double complex *pA;
    double complex *pB;

    for (i = 0; i < nrows; A += ncols, i++)
    {
        for (pB = B, j = 0; j < mrows; C++, j++)
        {
            for (pA = A, *C = 0.0, k  = 0; k < ncols; k++) *C += *pA++ * *pB++;
        }
    }
}

void CMatrix_x_Its_Transpose(double complex *C, double complex *A, int nrows, int ncols)
{
    int i,j,k;
    double complex *pAi0 = A;
    double complex *pAj0;
    double complex *pCi0 = C;
    double complex *pCji;

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

void CMatrix_Transposed_x_a_CMatrix(double complex *C, double complex *A, int nrows, int ncols, double complex *B, int mcols)
{
    int i,j,k;
    double complex *pA = A;
    double complex *pB;
    double complex *p_B;

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

void CMatrix_Transposed_x_the_CMatrix(double complex *C, double complex *A, int nrows, int ncols)
{
    double complex *pAki;
    double complex *pAkj;
    double complex *pA0i;
    double complex *pCij;
    double complex *pCji;
    double complex *pCii = C;
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

void CMatrix_x_a_CMatrix_Daggered(double complex *C, double complex *A, int nrows, int ncols, double complex *B, int mrows)
{
    int i,j,k;
    double complex *pA;
    double complex *pB;

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

void CMatrix_x_Its_Dagger(double complex *C, double complex *A, int nrows, int ncols)
{
    int i,j,k;
    double complex *pAi0 = A;
    double complex *pAj0;
    double complex *pCi0 = C;
    double complex *pCji;

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

void CMatrix_Daggered_x_a_CMatrix(double complex *C, double complex *A, int nrows, int ncols, double complex *B, int mcols)
{
   int i,j,k;
   double complex *pA = A;
   double complex *pB;
   double complex *p_B;

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

void CMatrix_Daggered_x_the_CMatrix(double complex *C, double complex *A, int nrows, int ncols)
{
    double complex *pAki;
    double complex *pAkj;
    double complex *pA0i;
    double complex *pCij;
    double complex *pCji;
    double complex *pCii = C;
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

void CMatrix_Daggered_x_an_RMatrix(double complex *C, double complex *A, int nrows, int ncols, double *B, int mcols)
{
    int i,j,k;
    double complex *pA = A;
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

void CMatrix_Transposed_x_an_RMatrix(double complex *C, double complex *A, int nrows, int ncols, double *B, int mcols)
{
    int i,j,k;
    double complex *pA = A;
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

void CMatrix_x_an_RMatrix_Transposed(double complex *C, double complex *A, int nrows, int ncols, double *B, int mrows)
{
    int i,j,k;
    double complex *pA;
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

void RMatrix_Transposed_x_a_CMatrix(double complex *C, double *A, int nrows, int ncols, double complex *B, int mcols)
{
    int i,j,k;
    double *pA = A;
    double complex *pB;
    double complex *p_B;

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

void RMatrix_x_a_CMatrix_Daggered(double complex *C, double *A, int nrows, int ncols, double complex *B, int mrows)
{
    int i,j,k;
    double *pA;
    double complex *pB;

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

void RMatrix_x_a_CMatrix_Transposed(double complex *C, double *A, int nrows, int ncols, double complex *B, int mrows)
{
    int i,j,k;
    double *pA;
    double complex *pB;

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
