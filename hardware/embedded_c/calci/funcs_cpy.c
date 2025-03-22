#define PI 3.14159265358979323846
#define H 0.01

double principal_range(double angle) {
    int k = (int)(angle / (2*PI));
    angle -= k * (2*PI);

    if (angle < 0) {
        angle += (2*PI);
    }

    return angle;
}

// Function for first derivative (y' = z)
double f1(double x, double y, double z) {
    return z;
}

// Function for second derivative (z' = -y)
double f2(double x, double y, double z) {
    return -y;
}

// Function to calculate sine using RK4 method
double sin(double x_target) {
    double x, y, z;
    double k1, k2, k3, k4, l1, l2, l3, l4;
    
    // Normalize the target angle to [0, 2π)
    x_target = principal_range(x_target);
    
    // Initial conditions for y = sin(x)
    x = 0.0;    // Starting x value
    y = 0.0;    // y(0) = 0
    z = 1.0;    // y'(0) = 1
    
    // RK4 method to solve the differential equation
    while(x < x_target) {
        // Ensure we don't overshoot the target
        if (x + H > x_target) {
            // Adjust the last step size to hit x_target exactly
            double last_h = x_target - x;
            
            // Calculate k values for y
            k1 = last_h * f1(x, y, z);
            l1 = last_h * f2(x, y, z);
            
            k2 = last_h * f1(x + last_h/2, y + k1/2, z + l1/2);
            l2 = last_h * f2(x + last_h/2, y + k1/2, z + l1/2);
            
            k3 = last_h * f1(x + last_h/2, y + k2/2, z + l2/2);
            l3 = last_h * f2(x + last_h/2, y + k2/2, z + l2/2);
            
            k4 = last_h * f1(x + last_h, y + k3, z + l3);
            l4 = last_h * f2(x + last_h, y + k3, z + l3);
            
            // Update y and z
            y = y + (k1 + 2*k2 + 2*k3 + k4)/6;
            z = z + (l1 + 2*l2 + 2*l3 + l4)/6;
            
            // Update x to target
            x = x_target;
        } else {
            // Regular step
            // Calculate k values for y
            k1 = H * f1(x, y, z);
            l1 = H * f2(x, y, z);
            
            k2 = H * f1(x + H/2, y + k1/2, z + l1/2);
            l2 = H * f2(x + H/2, y + k1/2, z + l1/2);
            
            k3 = H * f1(x + H/2, y + k2/2, z + l2/2);
            l3 = H * f2(x + H/2, y + k2/2, z + l2/2);
            
            k4 = H * f1(x + H, y + k3, z + l3);
            l4 = H * f2(x + H, y + k3, z + l3);
            
            // Update y and z
            y = y + (k1 + 2*k2 + 2*k3 + k4)/6;
            z = z + (l1 + 2*l2 + 2*l3 + l4)/6;
            
            // Update x
            x = x + H;
        }
    }
    
    return y;
}

double cos(double x){
    return sin((PI/2) - x);
}

double tan(double x){
    return sin(x)/cos(x);
}

// Define the differential equation: dy/dx = w * y / x
double f(double x, double y, double w) {
    return w * y / x;
}

// Function to calculate x^w using RK4 method
double pow(double x, double w) {
    if (x == 0) return 0;  // Handle special case
    if (w == 0) return 1;  // x^0 = 1
    
    // Starting point
    double x0 = 1.0;
    double y = 1.0;  // y(1) = 1
    
    // Calculate number of steps needed
    int steps = (int)((x - x0) / H);
    if (x < 1.0) {
        steps = (int)((x0 - x) / H);
        steps = -steps;  // Negative steps for x < 1
    }
    
    // Apply RK4 method
    for (int i = 0; i < steps; i++) {
        double k1 = H * f(x0, y, w);
        double k2 = H * f(x0 + 0.5 * H, y + 0.5 * k1, w);
        double k3 = H * f(x0 + 0.5 * H, y + 0.5 * k2, w);
        double k4 = H * f(x0 + H, y + k3, w);
        
        y += (k1 + 2 * k2 + 2 * k3 + k4) / 6;
        x0 += H;
    }
    
    // Handle any remaining fraction
    double remaining = x - x0;
    if (remaining > 0) {
        double k1 = remaining * f(x0, y, w);
        double k2 = remaining * f(x0 + 0.5 * remaining, y + 0.5 * k1, w);
        double k3 = remaining * f(x0 + 0.5 * remaining, y + 0.5 * k2, w);
        double k4 = remaining * f(x0 + remaining, y + k3, w);
        
        y += (k1 + 2 * k2 + 2 * k3 + k4) / 6;
    }
    
    return y;
}


// Define the differential equation: dy/dx = 1/x
double f_log(double x, double y) {
    return 1.0 / x;
}

// Function to calculate ln(x) using RK4 method
double ln(double x) {
    if (x <= 0) {
        return 0;
    }
    if (x <= 1){
        return -ln((double) 1/x);
    }
    
    // Starting point
    double x0 = 1.0;
    double y = 0.0;  // ln(1) = 0
    
    // Calculate number of steps needed
    int steps = (int)((x - x0) / H);
    if (x < 1.0) {
        steps = (int)((x0 - x) / H);
        steps = -steps;  // Negative steps for x < 1
    }
    
    // Apply RK4 method
    for (int i = 0; i < steps; i++) {
        double k1 = H * f_log(x0, y);
        double k2 = H * f_log(x0 + 0.5 * H, y + 0.5 * k1);
        double k3 = H * f_log(x0 + 0.5 * H, y + 0.5 * k2);
        double k4 = H * f_log(x0 + H, y + k3);
        
        y += (k1 + 2 * k2 + 2 * k3 + k4) / 6;
        x0 += H;
    }
    
    // Handle any remaining fraction
    double remaining = x - x0;
    if (remaining > 0) {
        double k1 = remaining * f_log(x0, y);
        double k2 = remaining * f_log(x0 + 0.5 * remaining, y + 0.5 * k1);
        double k3 = remaining * f_log(x0 + 0.5 * remaining, y + 0.5 * k2);
        double k4 = remaining * f_log(x0 + remaining, y + k3);
        
        y += (k1 + 2 * k2 + 2 * k3 + k4) / 6;
    }
    
    return y;
}

// Define the differential equation: dy/dx = 1 / sqrt(1 - x^2)
double f_arcsin(double x, double y) {
    return 1.0 / pow(1.0 - x * x, 0.5);
}

// Function to calculate arcsin(x) using RK4 method
double arcsin(double x) {
    if (x < -1 || x > 1) {
        return 0;
    }
    
    // Starting point
    double x0 = 0.0;
    double y = 0.0;  // arcsin(0) = 0
    
    // Calculate number of steps needed
    int steps = (int)((x - x0) / H);
    
    // Apply RK4 method
    for (int i = 0; i < steps; i++) {
        double k1 = H * f_arcsin(x0, y);
        double k2 = H * f_arcsin(x0 + 0.5 * H, y + 0.5 * k1);
        double k3 = H * f_arcsin(x0 + 0.5 * H, y + 0.5 * k2);
        double k4 = H * f_arcsin(x0 + H, y + k3);
        
        y += (k1 + 2 * k2 + 2 * k3 + k4) / 6;
        x0 += H;
    }
    
    // Handle any remaining fraction
    double remaining = x - x0;
    if (remaining != 0) {
        double k1 = remaining * f_arcsin(x0, y);
        double k2 = remaining * f_arcsin(x0 + 0.5 * remaining, y + 0.5 * k1);
        double k3 = remaining * f_arcsin(x0 + 0.5 * remaining, y + 0.5 * k2);
        double k4 = remaining * f_arcsin(x0 + remaining, y + k3);
        
        y += (k1 + 2 * k2 + 2 * k3 + k4) / 6;
    }
    
    return y;
}

double arccos(double x){
    return ((PI/2) - arcsin(x));
}

// Define the differential equation: dy/dx = 1 / (1 + x^2)
double f_arctan(double x, double y) {
    return 1.0 / (1.0 + x * x);
}

// Function to calculate arctan(x) using RK4 method
double arctan(double x) {
    // Starting point
    double x0 = 0.0;
    double y = 0.0;  // arctan(0) = 0
    
    // Calculate number of steps needed
    int steps = (int)((x - x0) / H);
    if (x < 0.0) {
        steps = (int)((x0 - x) / H);
        steps = -steps;  // Negative steps for x < 0
    }
    
    // Apply RK4 method
    for (int i = 0; i < steps; i++) {
        double k1 = H * f_arctan(x0, y);
        double k2 = H * f_arctan(x0 + 0.5 * H, y + 0.5 * k1);
        double k3 = H * f_arctan(x0 + 0.5 * H, y + 0.5 * k2);
        double k4 = H * f_arctan(x0 + H, y + k3);
        
        y += (k1 + 2 * k2 + 2 * k3 + k4) / 6;
        x0 += H;
    }
    
    // Handle any remaining fraction
    double remaining = x - x0;
    if (remaining != 0) {
        double k1 = remaining * f_arctan(x0, y);
        double k2 = remaining * f_arctan(x0 + 0.5 * remaining, y + 0.5 * k1);
        double k3 = remaining * f_arctan(x0 + 0.5 * remaining, y + 0.5 * k2);
        double k4 = remaining * f_arctan(x0 + remaining, y + k3);
        
        y += (k1 + 2 * k2 + 2 * k3 + k4) / 6;
    }
    
    return y;
}
