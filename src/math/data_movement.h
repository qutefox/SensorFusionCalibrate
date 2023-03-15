#pragma once

#include <string.h> // required for memcpy()

/*
* \brief Copy the mrows and mcols of the nrows x ncols matrix A starting with
*   A[row][col] to the submatrix S.
*   Note that S should be declared double S[mrows][mcols] in the calling
*   routine.
*
* Example:
*   #define N
*   #define M
*   #define NB
*   #define MB
*   double A[M][N],  B[MB][NB];
*   int row, col;
*
*   (your code to set the matrix A, the row number row and column number col)
*
*   if ( (row >= 0) && (col >= 0) && ((row + MB) < M) && ((col + NB) < N) )
*       Get_Submatrix(&B[0][0], MB, NB, &A[0][0], N, row, col);
*   printf("The submatrix B is \n"); ... }
*
* @param[in] S: Destination address of the submatrix.
* @param[in] mrows: The number of rows of the matrix S.
* @param[in] mcols: The number of columns of the matrix S.
* @param[in] A: Pointer to the first element of the matrix A[nrows][ncols]
* @param[in] ncols: The number of columns of the matrix A.
* @param[in] row: The row of A corresponding to the first row of S.
* @param[in] col: The column of A corresponding to the first column of S.
*/

void Get_Submatrix(double *S, int mrows, int mcols, double *A, int ncols, int row, int col);




