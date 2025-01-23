#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

// Returns a matrix with size mxn with all zero entries
float **mzeroes(int m, int n){
    float** mat = (float **) malloc(sizeof(float) * m);
    for(int i = 0; i < m; i++){
        mat[i] = (float *) malloc(sizeof(float) * n);
        for(int j = 0; j < n; j++) mat[i][j] = 0;
    }
    return mat;
}

// Returns the identity matrix with size mxm
float** meye(int m){
    float** mat = mzeroes(m, m);
    for(int i = 0; i < m; i++) mat[i][i] = 1.0;
    return mat;
}

// Prints the Matrix
void mprint(float **mat, int m, int n){
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++) printf("%.6lf ", mat[i][j]);
        if(i != m - 1) printf("\n");
    }
    printf("\n\n");
}

// Returns the copy of the matrix
float** mdup(float** mat, int m, int n){
    float** newmat = mzeroes(m , n);
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            newmat[i][j] = mat[i][j];
        }
    }
    return newmat;
}

// Swap two row of the matrix
void mrow_swap(float** mat, int n, int r1, int r2){
    for(int i = 0; i < n; i++){
        float temp = mat[r1][i];
        mat[r1][i] = mat[r2][i];
        mat[r2][i] = temp;
    }
}

// Returns the LU decomposition of 'mat' of size nxn
float ***LU(float **mat, int n){
    // Initializing L = I, U = mat
    float **L = meye(n);
    float **U = mdup(mat, n, n);

    int counter = 0;
    while(counter < n - 1){
        // Checking if the pivot's leftmost element is zero, if yes then swapping
        // with some non-zero pivot
        if(U[counter][counter] == 0){
            int check = 0;
            for(int i = counter + 1; i < n; i++){
                if(U[i][i] != 0){
                    mrow_swap(U, n, counter, i);
                    check = 1;
                }
            }

            if(check == 0){
                // No non-zero pivot found
                printf("LU decomposition failed.");
                return NULL;
            }
        }

        for(int i = counter + 1; i < n; i++){
            float multiplier = U[i][counter] / U[counter][counter];

            // Constructing L, L_ij = multiplier used to cancel A_ij
            L[i][counter] = multiplier;
            for(int j = counter; j < n; j++) U[i][j] -= multiplier * U[counter][j];
        }

        counter++;
    }
    

    float ***pair = (float ***) malloc(sizeof(float **) * 2);
    pair[0] = L;
    pair[1] = U;
    return pair;
}

int main(){
    int n = 2;
    float **A = mzeroes(n, n);
    A[0][0] = 3.0; A[0][1] = -1.0;
    A[1][0] = 9.0; A[1][1] = -3.0;

    float ***pair = LU(A, n);
    printf("L:\n");
    mprint(pair[0], n, n);
    printf("U:\n");
    mprint(pair[1], n, n);
    return 0;
}
