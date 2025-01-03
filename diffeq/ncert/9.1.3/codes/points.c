#include <stdlib.h>
#include <math.h>

float **diffEqPoints(int n, float h, float x, float y, float y1){ //taking initial values of z1, z2, z3, step value, 't' as input
  float **pts = (float **) malloc(sizeof(float *) * n);
  float y1_new;

  for (int i = 0; i < n; i++){
    pts[i] = (float *) malloc(sizeof(float) * 2);

    y1_new = y1 + h*(-pow(y1, 4)/(3*y));
    y = y + h*y1;
    y1 = y1_new;
    x = x + h;

    pts[i][0] = x;
    pts[i][1] = y;
  }

  return pts;
}

void freeMultiMem(float **points, int n){
  for(int i = 0; i < n; i++){
    free(points[i]);
  }

  free(points);
}
