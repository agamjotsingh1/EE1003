#include <stdlib.h>
#include <math.h>

// get 'n' points on the plot of the differential equation, 'h' being the step size, 'x', 'y1', 'y2' being the inital conditions
// y1 = y, y2 = y'
float **diffEqPoints(int n, float h, float x, float y1, float y2){
  float **pts = (float **) malloc(sizeof(float *) * n);

  // iteratively use euler's method with given parameters to return 'n' points in the plot of the differential equation
  for (int i = 0; i < n; i++){
    pts[i] = (float *) malloc(sizeof(float) * 2);

    float y2_new = y2 + h*(-pow(y2, 4)/(3*y1));
    y1 = y1 + h*y2;
    y2 = y2_new;
    x = x + h;

    pts[i][0] = x;
    pts[i][1] = y1;
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
