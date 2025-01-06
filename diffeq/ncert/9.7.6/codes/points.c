#include <stdlib.h>
#include <math.h>

// get 'n' points on the plot of the differential equation, 'h' being the step size, 'x', 'y1' being the inital conditions
// y1 = y
float **diffEqPoints(int n, float h, float x, float y1){
  float **pts = (float **) malloc(sizeof(float *) * n);

  // iteratively use euler's method with given parameters to return 'n' points in the plot of the differential equation
  for (int i = 0; i < n; i++){
    pts[i] = (float *) malloc(sizeof(float) * 2);

    y1 = y1 + h*(-((float) sqrt(1 - y1*y1))/((float) sqrt(1 - x*x)));
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
