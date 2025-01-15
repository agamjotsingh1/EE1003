#include <stdlib.h>
#include <math.h>

// get 'n' points on the plot of the differential equation, 'h' being the step size, 'x', 'y1', 'y2' being the inital conditions
// y1 = y, y2 = y'
float **diffEqPoints(int n, float h, float x, float y1, float y2){
  float **pts = (float **) malloc(sizeof(float *) * n);

  // iteratively use euler's method with given parameters to return 'n' points in the plot of the differential equation
  for (int i = 0; i < n; i++){
    pts[i] = (float *) malloc(sizeof(float) * 2);

    float y2_new = (y2*(4 + h*h) - 4*h*y1)/(4 + h*h);
    y1 = (y1*(4 + h*h) + 4*h*y2)/(4 + h*h);
    y2 = y2_new;
    x = x + h;

    pts[i][0] = x;
    pts[i][1] = y1;
  }

  return pts;
}

// get 'n' points on the plot of the differential equation, 'h' being the step size, 'x', 'y1', 'y2' being the inital conditions
// y1 = y, y2 = y'
float **diffEqPointsBilinear(int n, float h, float x, float y1, float y2){
  float **pts = (float **) malloc(sizeof(float *) * n);
  pts[0] = (float *) malloc(sizeof(float) * 2);
  pts[0][0] = x;
  pts[0][1] = y1;

  pts[1] = (float *) malloc(sizeof(float) * 2);
  pts[1][0] = x + h;
  x = x + h;
  pts[1][1] = y1 + h*y2;

  pts[2] = (float *) malloc(sizeof(float) * 2);
  pts[2][0] = x + h;
  x = x + h;
  pts[2][1] = (h*h*y2 - 2*h*y1 - y1)/(h*h + 4) - pts[0][1] - pts[1][1]*2*(h*h - 4)/(h*h + 4);

  // iteratively use bilinear-z transform method with given parameters to return 'n' points in the plot of the differential equation
  for (int i = 3; i < n; i++){
    pts[i] = (float *) malloc(sizeof(float) * 2);

    float y = - pts[i - 2][1] - pts[i - 1][1]*2*(h*h - 4)/(h*h + 4);

    pts[i][0] = x;
    pts[i][1] = y;

    x = x + h;
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
