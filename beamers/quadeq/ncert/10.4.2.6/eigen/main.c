#include <complex.h>
#include <stdio.h>
#include <stddef.h>
#include "impl/matrix.h"
#include "impl/hess.h"
#include "impl/givens.h"
#include "impl/schmidt.h"
#include "impl/eigen.h"

int main(){
    int m;
    printf("Enter the dimension of the matrix:\n");
    scanf("%d", &m);
    printf("Enter the values in the format {real} (whitespace) {imaginary}:\n");

    compl** mat = mzeroes(m, m);

    for(int i = 0; i < m; i++){
        for(int j = 0; j < m; j++){
            double a, b;
            scanf("%lf %lf", &a, &b);
            mat[i][j] = CMPLX(a, b);
        }
    }

    double tolerance = 1e-10;
    int max_iterations = 1000;
    int no_iterations = 0;
    compl* eigen_values = eigen_givens(mat, m, max_iterations, tolerance, &no_iterations);

    printf("Number of iterations: %d\n", no_iterations);
    printf("Eigen values:\n");
    for(int i = 0; i < m; i++) printf("%.6lf + %.6lfi\n", creal(eigen_values[i]), cimag(eigen_values[i]));
}