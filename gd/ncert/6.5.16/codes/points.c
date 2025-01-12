#include <stdlib.h>
#include <math.h>

float **objectiveFunctionPoints(int n, float h, float x){
    float **pts = (float **) malloc(sizeof(float *) * n);

    for (int i = 0; i < n; i++){
        pts[i] = (float *) malloc(sizeof(float) * 2);

        x = x + h;
        pts[i][0] = x;
        pts[i][1] = pow(x, 3) + pow(16 - x, 3);
    }

    return pts;
}

float gradientDescentPoint(int n, float h, float x, float tol){
    for (int i = 0; i < n; i++){
        if(fabs(3*pow(x, 2) - 3*pow(16 - x, 2)) < tol) break;
        x = (1 - 6*h)*x + 48*h;
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
