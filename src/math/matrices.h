#include <complex>

/*
* \brief Post multiply the nrows x ncols matrix A by the transpose of the
*   mrows x ncols matrix B to form the nrows x mrows matrix C,
*   i.e. C = A B', where ' denotes the transpose.
*   I.e. C = (Cij), where Cij = Sum (Aik Bjk) where the sum extends from
*   k = 0 to ncols - 1.
*
*   The matrix C should be declared as double C[nrows][mrows] in the
*   calling routine.  The memory allocated to C should not include any
*   memory allocated to A or B.
*
* Example:
*   #define N
*   #define M
*   #define MB
*   double A[M][N],  B[MB][N], C[M][MB];
*
*   (your code to initialize the matrices A and B)
*
*   Matrix_x_a_Matrix_Transposed(&C[0][0], &A[0][0], M, N, &B[0][0], NB)
*   printf("The matrix C = AB' is \n"); ...
*
* @param[in] C: Pointer to the first element of the matrix C.
* @param[in] A: Pointer to the first element of the matrix A.
* @param[in] nrows: The number of rows of matrices A and C.
* @param[in] ncols: The number of columns of the matrices A and B.
* @param[in] B: Pointer to the first element of the matrix B.
* @param[in] mrows: The number of rows of the matrix B and columns of the matrix C.
*/
void Matrix_x_a_Matrix_Transposed(double* C, double* A, int nrows, int ncols, double* B, int mrows);

/*
* \brief Post multiply an nrows x ncols matrix A by its transpose. The result
*   is an  nrows x nrows square symmetric matrix C, i.e. C = A A', where '
*   denotes the transpose.
*   I.e. C = (Cij), where Cij = Sum (Aik Ajk) where the sum extends from
*   k = 0 to ncols - 1.
*
*   The matrix C should be declared as double C[nrows][nrows] in the
*   calling routine.  The memory allocated to C should not include any
*   memory allocated to A.
*
* Example:
*   #define N
*   #define M
*   double A[M][N], C[M][M];
*
*   (your code to initialize the matrix A)
*
*   Matrix_x_Its_Transpose(&C[0][0], &A[0][0], M, N);
*   printf("The matrix C = AA ' is \n"); ...
*
* @param[in] C: Pointer to the first element of the matrix C.
* @param[in] A: Pointer to the first element of the matrix A.
* @param[in] nrows: The number of rows of matrix A.
* @param[in] ncols: The number of columns of the matrices A.
*/
void Matrix_x_Its_Transpose(double *C, double *A, int nrows, int ncols);

/*
* \brief Post multiply the transpose of the nrows x ncols matrix A by the
*   nrows x mcols matrix B to form the ncols x mcols matrix C,
*   i.e. C = A' B, where ' denotes the transpose.
*   I.e. C = (Cij), where Cij = Sum (Aki Bkj) where the sum extends
*   from k = 0 to nrows - 1.
*
*   The matrix C should be declared as double C[ncols][mcols] in the
*   calling routine.  The memory allocated to C should not include any
*   memory allocated to A or B.
*
* Example:
*   #define N
*   #define M
*   #define NB
*   double A[M][N],  B[M][NB], C[N][NB];
*
*   (your code to initialize the matrices A and B)
*
*   Matrix_Transposed_x_a_Matrix(&C[0][0], &A[0][0], M, N, &B[0][0], NB);
*   printf("The matrix C = A'B is \n"); ...
*
* @param[in] C: Pointer to the first element of the matrix C.
* @param[in] A: Pointer to the first element of the matrix A.
* @param[in] nrows: The number of rows of matrix A and the number of rows of the matrix B.
* @param[in] ncols: The number of columns of the matrices A and the number number of rows of the matrix C.
* @param[in] B: Pointer to the first element of the matrix B.
* @param[in] mcols: The number of columns of the matrices B and C.
*/
void Matrix_Transposed_x_a_Matrix(double *C, double *A, int nrows, int ncols, double *B, int mcols);

/*
* \brief Pre multiply an nrows x ncols matrix A by its transpose.  The result
*   is an ncols x ncols square symmetric matrix C, i.e. C = A' A, where '
*   denotes the transpose.  I.e. C = (Cij), where Cij = Sum (Aki Akj) where
*   the sum extends from k = 0 to nrows - 1.
*
*   The matrix C should be declared as double C[ncols][ncols] in the
*   calling routine.  The memory allocated to C should not include any
*   memory allocated to A.
*
* Example:
*   #define N
*   #define M
*   double A[M][N], C[N][N];
*
*   (your code to initialize the matrix A)
*
*   Matrix_Transposed_x_the_Matrix(&C[0][0], &A[0][0], M, N);
*   printf("The matrix C = A'A is \n"); ...
*
* @param[in] C: Pointer to the first element of the matrix C.
* @param[in] A: Pointer to the first element of the matrix A.
* @param[in] nrows: The number of rows of matrix A.
* @param[in] ncols: The number of columns of the matrices A.
*/
void Matrix_Transposed_x_the_Matrix(double*C, double *A, int nrows, int ncols);

/*
* \brief Post multiply the nrows x ncols _Complex matrix A by the transpose of
*   the mrows x ncols _Complex matrix B to form the nrows x mrows _Complex
*   matrix C, i.e. C = A B', where ' denotes the transpose.
*   I.e. C = (Cij), where Cij = Sum (Aik Bjk) where the sum extends from
*   k = 0 to ncols - 1.
*
*   The matrix C should be declared as std::complex<double> C[nrows][mrows] in
*   the calling routine.  The memory allocated to C should not include any
*   memory allocated to A or B.
*
* Example:
*   #define N
*   #define M
*   #define MB
*   std::complex<double> A[M][N], B[MB][N], C[M][MB];
*
*   (your code to initialize the matrices A and B)
*
*   CMatrix_x_a_CMatrix_Transposed(&C[0][0], &A[0][0], M, N, &B[0][0], NB);
*   printf("The matrix C = AB' is \n"); ...
*
* @param[in] C: Pointer to the first element of the matrix C.
* @param[in] A: Pointer to the first element of the matrix A.
* @param[in] nrows: The number of rows of matrices A and C.
* @param[in] ncols: The number of columns of the matrices A and B.
* @param[in] B: Pointer to the first element of the matrix B.
* @param[in] mrows: The number of rows of the matrix B and columns of the matrix C.
*/
void CMatrix_x_a_CMatrix_Transposed(std::complex<double> *C, std::complex<double> *A, int nrows, int ncols, std::complex<double> *B, int mrows);

/*
* \brief Post multiply an nrows x ncols _Complex matrix A by its transpose.  The
*   result is an  nrows x nrows square symmetric _Complex matrix C, i.e.
*   C = A A', where ' denotes the transpose.
*
*   The matrix C should be declared as std::complex<double> C[nrows][nrows] in
*   the calling routine.  The memory allocated to C should not include any
*   memory allocated to A.
*
* Example:
*   #define N
*   #define M
*   std::complex<double> A[M][N], C[M][M];
*
*   (your code to initialize the matrix A)
*
*   CMatrix_x_Its_Transpose(&C[0][0], &A[0][0], M, N);
*   printf("The matrix C = AA ' is \n"); ...
*
* @param[in] C: Pointer to the first element of the matrix C.
* @param[in] A: Pointer to the first element of the matrix A.
* @param[in] nrows: The number of rows of matrix A.
* @param[in] ncols: The number of columns of the matrices A.
*/
void CMatrix_x_Its_Transpose(std::complex<double> *C, std::complex<double> *A, int nrows, int ncols);

/*
* \brief Post multiply the transpose of the nrows x ncols _Complex matrix A by
*   the nrows x mcols _Complex matrix B to form the ncols x mcols _Complex
*   matrix C, i.e. C = A' B, where ' denotes the transpose.
*   I.e. C = (Cij), where Cij = Sum (Aki Bkj) where the sum extends
*   from k = 0 to nrows - 1.
*
*   The matrix C should be declared as std::complex<double> C[ncols][mcols] in
*   the calling routine.  The memory allocated to C should not include any
*   memory allocated to A or B.
*
* @param[in] C: Pointer to the first element of the matrix C.
* @param[in] A: Pointer to the first element of the matrix A.
* @param[in] nrows: The number of rows of matrix A and the number of rows of the matrix B.
* @param[in] ncols: The number of columns of the matrices A and the number of rows of the matrix C.
* @param[in] B: Pointer to the first element of the matrix B.
* @param[in] mcols: The number of columns of the matrices B and C.
*
* Example:
*   #define N
*   #define M
*   #define NB
*   std::complex<double> A[M][N],  B[M][NB], C[N][NB];
*
*   (your code to initialize the matrices A and B)
*
*   CMatrix_Transposed_x_a_CMatrix(&C[0][0], &A[0][0], M, N, &B[0][0], NB);
*   printf("The matrix C = A'B is \n"); ...
*/
void CMatrix_Transposed_x_a_CMatrix(std::complex<double> *C, std::complex<double> *A, int nrows, int ncols, std::complex<double> *B, int mcols);

/*
* \brief Pre multiply an nrows x ncols _Complex matrix A by its transpose.  The
*   result is an ncols x ncols square symmetric _Complex matrix C, i.e.
*   C = A' A, where ' denotes the transpose.  I.e. C = (Cij),
*   where Cij = Sum (Aki Akj) where the sum extends from
*   k = 0 to nrows - 1.
*
*   The matrix C should be declared as std::complex<double> C[ncols][ncols] in
*   the calling routine.  The memory allocated to C should not include any
*   memory allocated to A.
*
* Example:
*   #define N
*   #define M
*   std::complex<double> A[M][N], C[N][N];
*
*   (your code to initialize the matrix A)
*
*   CMatrix_Transposed_x_the_CMatrix(&C[0][0], &A[0][0], M, N);
*   printf("The matrix C = A'A is \n"); ...
*
* @param[in] C: Pointer to the first element of the matrix C.
* @param[in] A: Pointer to the first element of the matrix A.
* @param[in] nrows: The number of rows of matrix A.
* @param[in] ncols: The number of columns of the matrices A.
*/
void CMatrix_Transposed_x_the_CMatrix(std::complex<double> *C, std::complex<double> *A, int nrows, int ncols);

/*
* \brief Post multiply the nrows x ncols _Complex matrix A by the transpose
*   _Complex conjugate of the mrows x ncols _Complex matrix B to form the
*   nrows x mrows _Complex matrix C, i.e. C = A B', where ' denotes the
*   transpose _Complex conjugate .
*   I.e. C = (Cij), where Cij = Sum (Aik Bjk*) where the sum extends from
*   k = 0 to ncols - 1, and * denotes the _Complex conjugate.
*
*   The matrix C should be declared as std::complex<double> C[nrows][mrows] in
*   the calling routine.  The memory allocated to C should not include any
*   memory allocated to A or B.
*
* Example:
*   #define N
*   #define M
*   #define MB
*   std::complex<double> A[M][N], B[MB][N], C[M][MB];
*
*   (your code to initialize the matrices A and B)
*
*   CMatrix_x_a_CMatrix_Daggered(&C[0][0], &A[0][0], M, N, &B[0][0], NB);
*   printf("The matrix C = AB' is \n"); ...
*
* @param[in] C: Pointer to the first element of the matrix C.
* @param[in] A: Pointer to the first element of the matrix A.
* @param[in] nrows: The number of rows of matrices A and C.
* @param[in] ncols: The number of columns of the matrices A and B.
* @param[in] B: Pointer to the first element of the matrix B.
* @param[in] mrows: The number of rows of the matrix B and columns of the matrix C.
*/
void CMatrix_x_a_CMatrix_Daggered(std::complex<double> *C, std::complex<double> *A, int nrows, int ncols, std::complex<double> *B, int mrows);

/*
* \brief Post multiply an nrows x ncols _Complex matrix A by its _Complex
*   conjugate transposed.  The result is an  nrows x nrows square
*   Hermitian matrix C, i.e. C = A A', where ' denotes the conjugate
*   transpose.
*
*   The matrix C should be declared as std::complex<double> C[nrows][nrows] in
*   the calling routine.  The memory allocated to C should not include any
*   memory allocated to A.
*
* Example:
*   #define N
*   #define M
*   std::complex<double> A[M][N], C[M][M];
*
*   (your code to initialize the matrix A)
*
*   CMatrix_x_Its_Transpose(&C[0][0], &A[0][0], M, N);
*   printf("The matrix C = AA ' is \n"); ...
*
* @param[in] C: Pointer to the first element of the matrix C.
* @param[in] A: Pointer to the first element of the matrix A.
* @param[in] nrows: The number of rows of matrix A.
* @param[in] ncols: The number of columns of the matrices A.
*/
void CMatrix_x_Its_Dagger(std::complex<double> *C, std::complex<double> *A, int nrows, int ncols);

/*
* \brief Post multiply the transpose _Complex conjugate of the nrows x ncols
*   _Complex matrix A by the nrows x mcols _Complex matrix B to form the
*   ncols x mcols _Complex matrix C, i.e. C = A' B, where ' denotes the
*   transpose _Complex conjugate.
*   I.e. C = (Cij), where Cij = Sum (Aki^* Bkj) where the sum extends
*   from k = 0 to nrows - 1, and * denotes the _Complex conjugate.
*
*   The matrix C should be declared as std::complex<double> C[ncols][mcols] in
*   the calling routine.  The memory allocated to C should not include any
*   memory allocated to A or B.
*
* Example:
*   #define N
*   #define M
*   #define NB
*   std::complex<double> A[M][N],  B[M][NB], C[N][NB];
*
*   (your code to initialize the matrices A and B)
*
*   CMatrix_Daggered_x_a_CMatrix(&C[0][0], &A[0][0], M, N, &B[0][0], NB);
*   printf("The matrix C = A'B is \n"); ...
*
* @param[in] C: Pointer to the first element of the matrix C.
* @param[in] A: Pointer to the first element of the matrix A.
* @param[in] nrows: The number of rows of matrix A and the number of rows of the matrix B.
* @param[in] ncols: The number of columns of the matrices A and the number of rows of the matrix C.
* @param[in] B: Pointer to the first element of the matrix B.
* @param[in] mcols: The number of columns of the matrices B and C.
*/
void CMatrix_Daggered_x_a_CMatrix(std::complex<double> *C, std::complex<double> *A, int nrows, int ncols, std::complex<double> *B, int mcols);

/*
* \brief Pre multiply an nrows x ncols _Complex matrix A by its _Complex conjugate
*   transpose.  The result is an ncols x ncols square symmetric _Complex
*   matrix C, i.e. C = A' A, where ' denotes the conjugate transpose.
*   I.e. C = (Cij), where Cij = Sum (Aki^* Akj) where the sum extends from
*   k = 0 to nrows - 1 and Aki^* is the _Complex conjugate of Aki.
*
*   The matrix C should be declared as std::complex<double> C[ncols][ncols] in
*   the calling routine.  The memory allocated to C should not include any
*   memory allocated to A.
*
* Example:
*   #define N
*   #define M
*   std::complex<double> A[M][N], C[N][N];
*
*   (your code to initialize the matrix A)
*
*   CMatrix_Daggered_x_the_CMatrix(&C[0][0], &A[0][0], M, N);
*   printf("The matrix C = A'A is \n"); ...
*
* @param[in] C: Pointer to the first element of the matrix C.
* @param[in] A: Pointer to the first element of the matrix A.
* @param[in] nrows: The number of rows of matrix A.
* @param[in] ncols: The number of columns of the matrices A.
*/
void CMatrix_Daggered_x_the_CMatrix(std::complex<double> *C, std::complex<double> *A, int nrows, int ncols);

/*
* \brief Post multiply the transpose _Complex conjugate of the nrows x ncols
*   _Complex matrix A by the nrows x mcols real matrix B to form the
*   ncols x mcols _Complex matrix C, i.e. C = A' B, where ' denotes the
*   transpose _Complex conjugate.
*   I.e. C = (Cij), where Cij = Sum (Aki^* Bkj) where the sum extends
*   from k = 0 to nrows - 1, and * denotes the _Complex conjugate.
*
*   The matrix C should be declared as std::complex<double> C[ncols][mcols] in
*   the calling routine.  The memory allocated to C should not include any
*   memory allocated to A or B.
*
* Example:
*   #define N
*   #define M
*   #define NB
*   std::complex<double> A[M][N], C[N][NB];
*   double B[M][NB];
*
*   (your code to initialize the matrices A and B)
*
*   CMatrix_Daggered_x_an_RMatrix(&C[0][0], &A[0][0], M, N, &B[0][0], NB);
*   printf("The matrix C = A'B is \n"); ...
*
* @param[in] C: Pointer to the first element of the matrix C.
* @param[in] A: Pointer to the first element of the matrix A.
* @param[in] nrows: The number of rows of matrix A and the number of rows of the matrix B.
* @param[in] ncols: The number of columns of the matrices A and the number of rows of the matrix C.
* @param[in] B: Pointer to the first element of the matrix B.
* @param[in] mcols: The number of columns of the matrices B and C.
*/
void CMatrix_Daggered_x_an_RMatrix(std::complex<double> *C, std::complex<double> *A, int nrows, int ncols, double *B, int mcols);

/*
* \brief Post multiply the transpose of the nrows x ncols _Complex matrix A by
*   the nrows x mcols real matrix B to form the ncols x mcols _Complex
*   matrix C, i.e. C = A' B, where ' denotes the transpose.
*   I.e. C = (Cij), where Cij = Sum (Aki Bkj) where the sum extends
*   from k = 0 to nrows - 1.
*
*   The matrix C should be declared as std::complex<double> C[ncols][mcols] in
*   the calling routine.  The memory allocated to C should not include any
*   memory allocated to A or B.
*
* Example:
*   #define N
*   #define M
*   #define NB
*   std::complex<double> A[M][N], C[N][NB];
*   double B[M][NB];
*
*   (your code to initialize the matrices A and B)
*
*   CMatrix_Transposed_x_an_RMatrix(&C[0][0], &A[0][0], M, N, &B[0][0],NB);
*   printf("The matrix C = A'B is \n"); ...
*
* @param[in] C: Pointer to the first element of the matrix C.
* @param[in] A: Pointer to the first element of the matrix A.
* @param[in] nrows: The number of rows of matrix A and the number of rows of the matrix B.
* @param[in] ncols: The number of columns of the matrices A and the number of rows of the matrix C.
* @param[in] B: Pointer to the first element of the matrix B.
* @param[in] mcols: The number of columns of the matrices B and C.
*/
void CMatrix_Transposed_x_an_RMatrix(std::complex<double> *C, std::complex<double> *A, int nrows, int ncols, double *B, int mcols);

/*
* \brief Post multiply the nrows x ncols _Complex matrix A by the transpose of
*   the mrows x ncols real matrix B to form the nrows x mrows _Complex
*   matrix C, i.e. C = A B', where ' denotes the transpose.
*   I.e. C = (Cij), where Cij = Sum (Aik Bjk) where the sum extends from
*   k = 0 to ncols - 1.
*
*   The matrix C should be declared as std::complex<double> C[nrows][mrows] in
*   the calling routine.  The memory allocated to C should not include any
*   memory allocated to A or B.
*
* Example:
*   #define N
*   #define M
*   #define MB
*   std::complex<double> A[M][N], C[M][MB];
*   double B[MB][N];
*
*   (your code to initialize the matrices A and B)
*
*   CMatrix_x_an_RMatrix_Transposed(&C[0][0], &A[0][0], M, N, &B[0][0],NB);
*   printf("The matrix C = AB' is \n"); ...
*
* @param[in] C: Pointer to the first element of the matrix C.
* @param[in] A: Pointer to the first element of the matrix A.
* @param[in] nrows: The number of rows of matrices A and C.
* @param[in] ncols: The number of columns of the matrices A and B.
* @param[in] B: Pointer to the first element of the matrix B.
* @param[in] mrows: The number of rows of the matrix B and columns of the matrix C.
*/
void CMatrix_x_an_RMatrix_Transposed(std::complex<double> *C, std::complex<double> *A, int nrows, int ncols, double *B, int mrows);

/*
* \brief Post multiply the transpose of the nrows x ncols real matrix A by
*   the nrows x mcols _Complex matrix B to form the ncols x mcols _Complex
*   matrix C, i.e. C = A' B, where ' denotes the transpose.
*   I.e. C = (Cij), where Cij = Sum (Aki Bkj) where the sum extends
*   from k = 0 to nrows - 1.
*
*   The matrix C should be declared as std::complex<double> C[ncols][mcols] in
*   the calling routine.  The memory allocated to C should not include any
*   memory allocated to A or B.
*
* Example:
*   #define N
*   #define M
*   #define NB
*   std::complex<double> B[M][NB], C[N][NB];
*   double A[M][N];
*
*   (your code to initialize the matrices A and B)
*
*   RMatrix_Transposed_x_a_CMatrix(&C[0][0], &A[0][0], M, N, &B[0][0], NB);
*   printf("The matrix C = A'B is \n"); ...
*
* @param[in] C: Pointer to the first element of the matrix C.
* @param[in] A: Pointer to the first element of the matrix A.
* @param[in] nrows: The number of rows of matrix A and the number of rows of the matrix B.
* @param[in] ncols: The number of columns of the matrices A and the number of rows of the matrix C.
* @param[in] B: Pointer to the first element of the matrix B.
* @param[in] mcols: The number of columns of the matrices B and C.
*/
void RMatrix_Transposed_x_a_CMatrix(std::complex<double> *C, double *A, int nrows, int ncols, std::complex<double> *B, int mcols);

/*
* \brief Post multiply the nrows x ncols real matrix A by the transpose
*   _Complex conjugate of the mrows x ncols _Complex matrix B to form the
*   nrows x mrows _Complex matrix C, i.e. C = A B', where ' denotes the
*   transpose _Complex conjugate.
*   I.e. C = (Cij), where Cij = Sum (Aik Bjk*) where the sum extends from
*   k = 0 to ncols - 1, and * denotes the _Complex conjugate.
*
*   The matrix C should be declared as std::complex<double> C[nrows][mrows] in
*   the calling routine.  The memory allocated to C should not include any
*   memory allocated to A or B.
*
* Example:
*   #define N
*   #define M
*   #define MB
*   std::complex<double> B[MB][N], C[M][MB];
*   double A[M][N];
*
*   (your code to initialize the matrices A and B)
*
*   RMatrix_x_a_CMatrix_Daggered(&C[0][0], &A[0][0], M, N, &B[0][0], NB);
*   printf("The matrix C = AB' is \n"); ...
*
* @param[in] C: Pointer to the first element of the matrix C.
* @param[in] A: Pointer to the first element of the matrix A.
* @param[in] nrows: The number of rows of matrices A and C.
* @param[in] ncols: The number of columns of the matrices A and B.
* @param[in] B: Pointer to the first element of the matrix B.
* @param[in] mrows: The number of rows of the matrix B and columns of the matrix C.
*/
void RMatrix_x_a_CMatrix_Daggered(std::complex<double> *C, double *A, int nrows, int ncols, std::complex<double> *B, int mrows);

/*
* \brief Post multiply the nrows x ncols real matrix A by the transpose of
*   the mrows x ncols _Complex matrix B to form the nrows x mrows _Complex
*   matrix C, i.e. C = A B', where ' denotes the transpose.
*   I.e. C = (Cij), where Cij = Sum (Aik Bjk) where the sum extends from
*   k = 0 to ncols - 1.
*
*   The matrix C should be declared as std::complex<double> C[nrows][mrows] in
*   the calling routine.  The memory allocated to C should not include any
*   memory allocated to A or B.
*
* Example:
*   #define N
*   #define M
*   #define MB
*   std::complex<double> B[MB][N], C[M][MB];
*   double A[M][N];
*
*   (your code to initialize the matrices A and B)
*
*   RMatrix_x_a_CMatrix_Transposed(&C[0][0], &A[0][0], M, N, &B[0][0], NB);
*   printf("The matrix C = AB' is \n"); ...
*
* @param[in] C: Pointer to the first element of the matrix C.
* @param[in] A: Pointer to the first element of the matrix A.
* @param[in] nrows: The number of rows of matrices A and C.
* @param[in] ncols: The number of columns of the matrices A and B.
* @param[in] B: Pointer to the first element of the matrix B.
* @param[in] mrows: The number of rows of the matrix B and columns of the matrix C.
*/
void RMatrix_x_a_CMatrix_Transposed(std::complex<double> *C, double *A, int nrows, int ncols, std::complex<double> *B, int mrows);
