#include <complex.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

// Gives the givens matrix associated with vector 'vec' at position (i, j)
compl** g_mat(int m, int i, int j, compl** vec, double tolerance){
    compl** mat = meye(m);
    compl xi = vec[i][0];
    compl xj = vec[j][0];
    
    compl c = 1 + 0*I, s = 0 + 0*I;

    if(cabs(xi) > tolerance && cabs(xj) > tolerance){
        c = conj(xi)/sqrt(pow(cabs(xi), 2) + pow(cabs(xj), 2));
        s = conj(xj)/sqrt(pow(cabs(xi), 2) + pow(cabs(xj), 2));
    }

    mat[i][i] = c;
    mat[i][j] = s;
    mat[j][i] = -conj(s);
    mat[j][j] = conj(c);
    return mat;
}

// Gives QR decomposition of the hessenberg matrix H
compl** givens(compl** H, int m, double tolerance){
    for(int i = 0; i < m - 1; i++){
        compl** vec = mgetcol(H, m, m, i);
        compl** G = g_mat(m, i, i + 1, vec, tolerance);
        H = mmul(G, H, m, m, m); 
        H = mmul(H, mT(G, m, m), m, m, m); 
    }

    return H;
}