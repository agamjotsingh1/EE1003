#include <complex.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

// Computes the QR decomposition using gram-schmidt algorithm
compl** schmidt(compl** A, int m, int n, double tolerance){
    compl** Q = mzeroes(m, n);

    for(int i = 0; i < n; i++){
        compl** a = mgetcol(A, m, n, i);
        compl** q = mdup(a, m, 1);

        for(int j = 0; j < i; j++){
            compl** col = mgetcol(Q, m, n, j);
            compl x = mmul(mT(col, m, 1), a, 1, m, 1)[0][0];
            q = madd(q, mscale(col, m, 1, -x), m, 1);
        }
        
        if(!is_zero(q, m, 1, tolerance)) q = mscale(q, m, 1, (compl) 1/vnorm(q, m));

        for(int j = 0; j < m; j++){
            Q[j][i] = q[j][0];
        }
    }

    A = mmul(mT(Q, m, n), A, m, n, m);
    A = mmul(A, Q, m, n, m);
    return A;
}