#include "data_movement.h"

void Get_Submatrix(double *S, int mrows, int mcols, double *A, int ncols, int row, int col)
{
    int number_of_bytes = sizeof(double) * mcols;
    for (A += row * ncols + col; mrows > 0; A += ncols, S+= mcols, mrows--)
    {
        memcpy(S, A, number_of_bytes);
    }
}
