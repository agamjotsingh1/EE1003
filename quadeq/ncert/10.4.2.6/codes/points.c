#include <stdlib.h>
#include <math.h>

// Plot 'n' points in the quadratic equation, with 'h' as step size, 'x' as the initial value
float **quadPlot(int n, float h, float x){
    float **pts = (float **) malloc(sizeof(float *) * n);

    for (int i = 0; i < n; i++){
        pts[i] = (float *) malloc(sizeof(float) * 2);

        x = x + h;
        pts[i][0] = x;
        pts[i][1] = 2*x*x + 3*x - 90;
    }

    return pts;
}

// Get the root of the quadratic equation using Newton-Raphson method, with intial guess as 'x' and max iterations as 'maxIters', tolerance as 'tol'
float quadRoot(int maxIters, float tol, float x){
    for (int i = 0; i < maxIters; i++){
        if(fabs((2*x*x + 90)/(4*x + 3) - x) < tol) break;
        x = (2*x*x + 90)/(4*x + 3);
    }

    return x;
}

// free a 2 dimentional array 'points' with 'n' rows in memory
void freeMultiMem(float **points, int n){
    for(int i = 0; i < n; i++){
        free(points[i]);
    }

    free(points);
}
