#include <QtTest>

#include "../src/math/matrices.h"

class matrices : public QObject
{
    Q_OBJECT

public:
    matrices();
    ~matrices();

private slots:
    void test_case1();

};

matrices::matrices()
{

}

matrices::~matrices()
{

}

void print_matrix(double *X, int m, int n) {
   int i,j;

   for (i = 0; i < m; i++) {
      for (j = 0; j < n; j++) printf(" %8.2f ",*X++);
      printf("\n");
   }
}

void test_matrix(double *Z, double *X, int ma, int na, double *Y, int nb, char cz, char cx, char cy)
{
   int i,j;

   for (i = 0; i < ma; i++)
      for (j = 0; j < na; j++) {
         X[na*i+j] = (double) (na * i + j + 1);
      }

   for (i = 0; i < nb; i++)
      for (j = 0; j < na; j++) {
         Y[na*i+j] = (double) (na * i + j + j + 1);
      }

   Matrix_x_a_Matrix_Transposed(Z, X, ma, na, Y, nb);

   printf("\nThe Matrix %c\n",cx);
   print_matrix( X, ma, na);
   printf("\nThe Matrix %c\n",cy);
   print_matrix( Y, nb, na);
   printf("\nThe Matrix %c = %c%c', where ' is the transpose\n",cz, cx, cy);
   print_matrix( Z, ma, nb);
}

void matrices::test_case1()
{
    double A[3][2], B[3][2], C[3][3];
    double D[2][3], E[2][3], F[2][2];
    double G[3][2], H[2][2], I[3][2];
    double J[2][2], K[2][2], L[2][2];

    test_matrix((double *)C, (double *)A, 3, 2, (double *)B, 3, 'C','A','B');
    test_matrix((double *)F, (double *)D, 2, 3, (double *)E, 2, 'F','D','E');
    test_matrix((double *)I, (double *)G, 3, 2, (double *)H, 2, 'I','G','H');
    test_matrix((double *)L, (double *)J, 2, 2, (double *)K, 2, 'L','J','K');
}

QTEST_APPLESS_MAIN(matrices)

#include "tst_matrices.moc"
