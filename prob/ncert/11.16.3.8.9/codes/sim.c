#include <stdlib.h>
#include <openssl/rand.h>

// Generates a random float between 0 and 1
float rand_float(){
    unsigned char buf;

    // Generate random value between 1 and 256
    if (RAND_bytes(&buf, 1) != 1) {
        printf("Error generating random byte.");
        exit(-1);
    } 

    // Normalize the value to make it between 0 and 1
    return (float) (buf)/255;
}

// Generates a random bernoulli random variable
// 0 -> heads, 1 -> tails
int rand_bernoulli(float p){
    return rand_float() < p ? 0 : 1;
}

// cdf plot with given bernoulli variable (number of tails)
// n -> no of tosses, iters -> number of iters, p -> probability of heads
float *binomCdfPlot(int n, int iters, float p){
    float *pts = (float *) malloc(sizeof(float *) * (n + 1)); 
    for(int i = 0; i < n + 1; i++) pts[i] = 0;

    for(int i = 0; i < iters; i++){
        // creating 'n' random tosses with bernoulli random variable and adding their sum
        // sum is the number of tails in this trial
        int sum = 0;
        for(int j = 0; j < n; j++) sum += rand_bernoulli(p);

        // for each after random variable after 'sum', we add 1 for cummulating all the probabilities
        for(int j = n; j >= sum; j--) pts[j] += 1;
    }

    // normalizing the cdf
    for(int i = 0; i < n + 1; i++) pts[i] /= iters;
    return pts;
}

// pmf plot with given bernoulli variable (number of tails)
// n -> no of tosses, iters -> number of iters, p -> probability of heads
float *binomPmfPlot(int n, int iters, float p){
    float *pts = (float *) malloc(sizeof(float *) * (n + 1)); 
    for(int i = 0; i < n + 1; i++) pts[i] = 0;

    for(int i = 0; i < iters; i++){
        // creating 'n' random tosses with bernoulli random variable and adding their sum
        int sum = 0;
        for(int j = 0; j < n; j++) sum += rand_bernoulli(p);

        pts[sum] += 1;
    }

    // normalizing the pmf
    for(int i = 0; i < n + 1; i++) pts[i] /= iters;
    return pts;
}

// free a 1 dimentional array 'arr'
void freeSingleMem(float *arr){
    free(arr);
}

// free a 2 dimentional array 'points' with 'n' rows in memory
void freeMultiMem(float **points, int n){
    for(int i = 0; i < n; i++){
        free(points[i]);
    }

    free(points);
}
