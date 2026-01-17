#include <stdint.h>

#define PI 3.14159265358979323846
#define H 0.01
#define MAX_ITERS 100000

double fast_inv_sqrt(double x){
    if(x <= 0) return 0;
    x = (float) x;
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = x * 0.5F;
	y  = x;
	i  = * ( long * ) &y;
	i  = 0x5f3759df - ( i >> 1 );
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );

	return (double) y;
}

double principal_range(double angle) {
    int k = (int)(angle / (2*PI));
    angle -= k * (2*PI);
    if (angle < 0) angle += (2*PI);
    return angle;
}

// Sine calculation using RK4 for y'' = -y
double sin(double x_target) {
    x_target = principal_range(x_target);
    double x = 0.0, y = 0.0, z = 1.0;
    
    while(x < x_target) {
        double h = (x + H > x_target) ? (x_target - x) : H;
        
        double k1 = h * z;
        double l1 = h * (-y);
        
        double k2 = h * (z + l1/2);
        double l2 = h * (-(y + k1/2));
        
        double k3 = h * (z + l2/2);
        double l3 = h * (-(y + k2/2));
        
        double k4 = h * (z + l3);
        double l4 = h * (-(y + k3));
        
        y += (k1 + 2*k2 + 2*k3 + k4)/6;
        z += (l1 + 2*l2 + 2*l3 + l4)/6;
        x += h;
    }
    
    return y;
}

double cos(double x) {
    return sin(PI/2 - x);
}

double tan(double x){
    return sin(x)/cos(x);
}

// Power function using RK4 for dy/dx = w*y/x
double pow(double x, double w) {
    if(x < 0 && ((int) w) != w) return 0;
    if (x == 0) return 0;
    if (w == 0) return 1;
    
    if(x < 0) return ( ((int) w)%2 == 0 ? 1: -1)*pow(-x, w);

    double x0 = 1.0, y = 1.0;
    double target = x;
    int steps = (int)((target - x0) / H);
    
    for (int i = 0; i < steps; i++) {
        double k1 = H * w * y / x0;
        double k2 = H * w * (y + k1/2) / (x0 + H/2);
        double k3 = H * w * (y + k2/2) / (x0 + H/2);
        double k4 = H * w * (y + k3) / (x0 + H);
        
        y += (k1 + 2*k2 + 2*k3 + k4) / 6;
        x0 += H;
    }
    
    return y;
}

// Natural log using RK4 for dy/dx = 1/x
double ln(double x) {
    if (x <= 0) return 0.0;
    if (x < 1) return -ln(1/x);
    //return (0.1)*2.3016;
    
    double x0 = 1.0, y = 0.0;
    int steps = (int)((x - x0) / H);
    
    for (int i = 0; i < steps; i++) {
        double k1 = H / x0;
        double k2 = H / (x0 + H/2);
        double k3 = H / (x0 + H/2);
        double k4 = H / (x0 + H);
        
        y += (k1 + 2*k2 + 2*k3 + k4) / 6;
        x0 += H;
    }
    
    return y;
}

// Arctan using RK4 for dy/dx = 1/(1+x²)
double arctan(double x) {
    double x0 = 0.0, y = 0.0;
    int steps = x >= 0 ? (int) (x / H): (int) (-x / H); 
    double step_dir = (x >= 0) ? 1 : -1;
    
    for (int i = 0; i < steps; i++) {
        double k1 = H / (1 + x0*x0);
        double k2 = H / (1 + (x0 + H/2)*(x0 + H/2));
        double k3 = H / (1 + (x0 + H/2)*(x0 + H/2));
        double k4 = H / (1 + (x0 + H)*(x0 + H));
        
        y += step_dir * (k1 + 2*k2 + 2*k3 + k4) / 6;
        x0 += step_dir * H;
    }
    
    return y;
}

// Arcsin using RK4 for dy/dx = 1/sqrt(1-x²)
double arcsin(double x) {
    if (x < -1 || x > 1) return 0;

    double x0 = 0.0, y = 0.0;
    int steps = x >= 0 ? (int) (x / H): (int) (-x / H); 
    double step_dir = (x >= 0) ? 1 : -1;

    for (int i = 0; i < steps; i++) {
        double k1 = H * fast_inv_sqrt(1 - x0*x0);
        double k2 = H * fast_inv_sqrt(1 - (x0 + step_dir*H/2)*(x0 + step_dir*H/2));
        double k3 = H * fast_inv_sqrt(1 - (x0 + step_dir*H/2)*(x0 + step_dir*H/2));
        double k4 = H * fast_inv_sqrt(1 - (x0 + step_dir*H)*(x0 + step_dir*H));

        y += step_dir * (k1 + 2*k2 + 2*k3 + k4) / 6;
        x0 += step_dir * H;
    }

    return y;
}

double arccos(double x){
    return ((PI/2) - arcsin(x));
}

double factorial(double n) {
    if(n < 0) return 0;
    if(n == 0 || n == 1) return 1;

    double result = 1.0;
    
    for(double i = 2; i <= (int) n; i++) {
        result *= i;
    }
    
    return result;
}

int gcd(int a, int b) {
    int temp;

    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }

    return a;
}

void dtf(double decimal, long int* numerator, long int* denominator) {
    *numerator = (int)(decimal * 1000000);
    *denominator = 1000000;
    
    int gcd_ = gcd(*numerator, *denominator);
    
    *numerator = *numerator/gcd_;
    *denominator = *denominator/gcd_;
}
