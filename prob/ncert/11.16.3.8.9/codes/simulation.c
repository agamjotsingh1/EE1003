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
int rand_bernoulli(float p){
    return rand_float() < p ? 0 : 1;
}

// Gives the relative frequency plot with 'n' trials, 'm' tosses, 'p' is the probability of heads
float **simPlot(int n, int m, float p){
    float **pts = (float **) malloc(sizeof(float *) * n); 
    int count = 0;

    for(int i = 0; i < n; i++){
        pts[i] = (float *) malloc(sizeof(float) * 2);

        // Seperate case for i = 0, as division by zero error will occur
        if(i == 0){
            pts[0][0] = 0;
            pts[0][1] = 1;
            continue;
        }

        // creating 'm' random tosses with bernoulli random variable
        int *tosses = (int *) malloc(sizeof(int) * m);
        for(int j = 0; j < m; j++) tosses[j] = rand_bernoulli(p);

        int isFavourable = 1;

        for(int j = 0; j < m; j++) {
            if(tosses[j] == 0){
                isFavourable = 0;
                break;
            }
        }

        // if this trial is favourable then increase count
        if(isFavourable) count++;
        free(tosses);

        pts[i][0] = i;
        pts[i][1] = 1 - ((float) count/i);
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
