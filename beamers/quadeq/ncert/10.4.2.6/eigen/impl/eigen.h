#include <complex.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

int is_triangular(compl** mat, int m, double tolerance){
    for(int i = 0; i < m - 1; i++){
        if(cabs(mat[i + 1][i]) > tolerance) return 0;
    }

    return 1;
}

void shift(compl** mat, int m, compl s){
    for(int i = 0; i < m; i++){
        mat[i][i] += s;
    }
}

compl* eigen_givens(compl** mat, int m, int max_iterations, double tolerance, int* no_iterations){
    // Computing the hessenberg form
    mat = hess(mat, m, tolerance);
    int i = 0;

    for(int n = m; n > 1; n--){
        while(i < max_iterations){
            if(is_triangular(mat, m, tolerance)) break;
            // Computing the shift amount
            compl rayleigh_shift = mat[n - 1][n - 1];

            shift(mat, m, -rayleigh_shift);
            // Doing the givens rotation
            mat = givens(mat, m, tolerance);
            shift(mat, m, rayleigh_shift);

            i++;
            if(cabs(mat[n - 1][n - 2]) < tolerance) break;
        }
    }

    *no_iterations = i;

    compl* eigen_values = malloc(sizeof(compl)*m);

    // Solving Jordan Blocks
    for(int i = 0; i < m; i++) {
        if(i + 1 < m && cabs(mat[i + 1][i]) > tolerance){
            compl a = mat[i][i], b = mat[i][i + 1], c = mat[i + 1][i], d = mat[i + 1][i + 1];
            compl D = (a + d)*(a + d) - 4*(a*d - b*c);
            
            eigen_values[i] = ((a + d) + csqrt(D))/2;
            eigen_values[i + 1] = ((a + d) - csqrt(D))/2;
            i++;
        }
        else {
            eigen_values[i] = mat[i][i];
        }
    }

    return eigen_values;
}