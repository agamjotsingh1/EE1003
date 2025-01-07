#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265358979323846 

// get 'n' points within x = x1 to x = x2 and line equation as ax + by + c = 0
float **lineGet(int n, float x1, float x2, float a, float b, float c) {
    float **pts = (float **) malloc(sizeof(float *) * n); 

    for(int i = 0; i < n; i++){
        pts[i] = (float *) malloc(sizeof(float) * 2);

        // diving the line into 'n' parts and increasing x appropriately
        pts[i][0] = x1 + ((x2 - x1)*i)/n; 
        pts[i][1] = -(c + a*pts[i][0])/b; 
    }

    return pts;
}

// get 'n' points on a circle with center (x, y) and radius r
float **circleGet(int n, float x, float y, float r) {
    float **pts = (float **) malloc(sizeof(float *) * n); 
    float theta = 0;
    for(int i = 0; i < n; i++){
        pts[i] = (float *) malloc(sizeof(float) * 2);

        // plotting as parametric form of the equation with radius r and center (x. y) and parameter theta
        pts[i][0] = x + r*cos(theta);
        pts[i][1] = y + r*sin(theta);
        theta += 2*PI/n;
    }
    return pts;
}

// fill the sector with angle 'phi' and 'm' radius vectors of 'n' points on each radius vector, radius r, center (x, y)
float **fillSector(int m, int n, float phi, float x, float y, float r) {
    float **pts = (float **) malloc(sizeof(float *) * n * m); 
    float theta = 0;
    int i = 0;

    while (theta < phi){
        // plotting as parametric form of the radius vector with radius r and center (x. y) and parameter theta
        float **linePts = lineGet(n, x, x + r*cos(theta), tan(theta), -1.0, y - x*tan(theta));
        for(int j = 0; j < n; j++){
            pts[i] = (float *) malloc(sizeof(float) * 2);
            pts[i][0] = linePts[j][0];
            pts[i][1] = linePts[j][1];
            i++;
        }
        free(linePts);

        theta += (float) phi/m;
    }

    return pts;
}

// find area using trapezoid method between x = 'a' and 'b'
float areaTrapezoidLine(float a, float b, float h) {
    float A = 0, x = a;
    int n = floor((b - a)/h);

    for(int i = 0; i <= n; i++){
        A += h*(x/sqrt(3)) + h*h/2*(sqrt(3));
        x += h;
    }

    return A;
}

// find area using trapezoid method between x = 'a' and 'b'
float areaTrapezoidCirc(float a, float b, float h) {
    float A = 0, x = a;
    int n = floor((b - a)/h);

    for(int i = 0; i < n; i++){
        A += h*(sqrt(4 - x*x)) + h*h/2*(-x/sqrt(4 - x*x));
        x += h;
    }

    return A;
}

// free a 2 dimentional array 'points' with 'n' rows in memory
void freeMultiMem(float **points, int n){
  for(int i = 0; i < n; i++){
    free(points[i]);
  }

  free(points);
}
