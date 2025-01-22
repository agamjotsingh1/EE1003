#include <complex.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

// Computes the Hessenberg form of the matrix 'A'
compl** hess(compl** A, int m, double tolerance){
    for(int i = 0; i < m - 2; i++){
        compl** P_i = meye(m);
        int m_sub = m - i - 1;

        compl** x = mzeroes(m_sub, 1);
        for(int k = i + 1; k < m; k++) x[k - i - 1][0] = A[k][i];

        compl rho;
        if(cabs(x[0][0]) < tolerance) rho = 1;
        else rho = -(x[0][0])/((compl) cabs(x[0][0]));

        compl** u = madd(x, mscale(e(m_sub, 1), m_sub, 1, -rho*vnorm(x, m_sub)), m_sub, 1);

        if(vnorm(u, m_sub) > tolerance) u = mscale(u, m_sub, 1, 1/vnorm(u, m_sub));

        // P_sub = I - 2uu*
        compl** P_sub = madd(meye(m_sub), mscale(mmul(u, mT(u, m_sub, 1), m_sub, 1, m_sub), m_sub, m_sub, -2), m_sub, m_sub);

        for(int j = i + 1; j < m; j++){
            for(int k = i + 1; k < m; k++){
                P_i[j][k] = P_sub[j - i - 1][k - i - 1];
            }
        }

        A = mmul(A, P_i, m, m, m);
        A = mmul(P_i, A, m, m, m);
    }

    return A;
}