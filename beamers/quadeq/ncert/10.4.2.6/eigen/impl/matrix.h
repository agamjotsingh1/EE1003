#include <complex.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

typedef double complex compl;

// Prints the Matrix
void mprint(compl** mat, int m, int n){
    printf("[");
    for(int i = 0; i < m; i++){
        printf("[");
        for(int j = 0; j < n; j++){
            printf("%.6lf + %.6lfi", creal(mat[i][j]), cimag(mat[i][j]));
            if(j != n - 1) printf(", ");
        }
        printf("]");
        if(i != m - 1) printf("\n");
    }
    printf("]\n\n");
}

// Returns a matrix with size mxn with all zero entries
compl** mzeroes(int m, int n){
    compl** mat = (compl**) malloc(sizeof(compl)*m);
    for(int i = 0; i < m; i++){
        mat[i] = (compl*) malloc(sizeof(compl)*n);

        for(int j = 0; j < n; j++){
            mat[i][j] = 0 + 0*I;
        }
    }
    return mat;
}

// Checks equality of matrices
int mequals(compl** mat1, compl** mat2, int m, int n){
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            if(mat1[i][j] != mat2[i][j]) return 0;
        }
    }
    return 1;
}

// Returns the identity matrix with size mxm
compl** meye(int m){
    compl** mat = mzeroes(m, m);
    for(int i = 0; i < m; i++) mat[i][i] = 1 + 0*I;
    return mat;
}

// Returns the scaled matrix with scaling factor as k
compl** mscale(compl** mat, int m, int n, compl k){
    compl** newmat = mzeroes(m, n);
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            newmat[i][j] = k*(mat[i][j]);
        }
    }
    return newmat;
}

// Adds two matrices and returns it
compl** madd(compl** mat1, compl** mat2, int m, int n){
    compl** newmat = mzeroes(m, n);
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            newmat[i][j] = mat1[i][j] + mat2[i][j];
        }
    }
    return newmat;
}

// Takes complex transpose of the matrix and returns it
compl** mT(compl** mat, int m, int n){
    compl** newmat = mzeroes(n, m);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            newmat[i][j] = conj(mat[j][i]);
        }
    }
    return newmat;
}

// Returns the multiplication of two matrices with size mxn and nxr
compl** mmul(compl** mat1, compl** mat2, int m, int n, int r){
    compl** newmat = mzeroes(m , r);

    for(int i = 0; i < m; i++){
        for(int j = 0; j < r; j++){
            for(int k = 0; k < n; k++) {
                newmat[i][j] += mat1[i][k]*mat2[k][j];
            }
        }
    }

    return newmat;
}

// Returns the 'k'th column of the matrix of size mxn
compl** mgetcol(compl** mat, int m, int n, int k){
    compl** newmat = mzeroes(m , 1);
    for(int i = 0; i < m; i++){
        newmat[i][0] = mat[i][k]; 
    }

    return newmat;
}

// Returns the copy of the matrix
compl** mdup(compl** mat, int m, int n){
    compl** newmat = mzeroes(m , n);
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            newmat[i][j] = mat[i][j];
        }
    }
    return newmat;
}

// Gives the norm of a m dimensional vector
double vnorm(compl** vec, int m){
    compl** norm = mmul(mT(vec, m, 1), vec, 1, m, 1);
    return sqrt(creal(norm[0][0]));
} 

// Returns the m dimensional impulse vector with 1 at 'i'th position
compl** e(int m, int i){
    compl** mat = mzeroes(m, 1);
    mat[i-1][0] = 1;
    return mat;
}

// Checks if matrix is zero
int is_zero(compl** mat, int m, int n, double tolerance){
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            if(cabs(mat[i][j]) > tolerance) return 0;
        }
    }
    return 1;
}