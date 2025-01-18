#include <stdlib.h>
#include <math.h>

// Get 'n' points on the line ax + by + c = 0, between x1 and x2
float **lineGet(int n, float x1, float x2, float a, float b, float c) {
    float **pts = (float **) malloc(sizeof(float *) * n); 

    for(int i = 0; i < n; i++){
        pts[i] = (float *) malloc(sizeof(float) * 2 * n);
        pts[i][0] = x1 + ((x2 - x1)*i)/n; 
        pts[i][1] = -(c + a*pts[i][0])/b; 
    }

    return pts;
}

// free a 2 dimentional array 'points' with 'n' rows in memory
void freeMultiMem(float **points, int n){
    for(int i = 0; i < n; i++){
        free(points[i]);
    }

    free(points);
}
