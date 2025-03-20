#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "common_threads.h"

typedef struct {
    int num;
    int* data;
} myarg_t;

typedef struct {
    float avg;
} myret_t;

void *mythread(void *arg) {
    myarg_t *args = (myarg_t *) arg;
    printf("Averaging %d integers.\n", args->num);
    float sum = 0.0;
    myret_t* rvals = malloc(sizeof(myret_t));
    assert(rvals != NULL);

    for (int i = 0; i < args->num; i++) {
        sum += args->data[i];
    }

    rvals->avg = sum / args->num;
    return (void*) rvals;
}

int main(int argc, char *argv[]) {

    srand(time(NULL));

    if (argc != 2) {
        printf("Error: requires a number of elements.\n");
        exit(EXIT_FAILURE);
    }

    int num_elements = atoi(argv[1]);
    int* data = malloc(sizeof(int) * num_elements);
    for (int i = 0; i < num_elements; i++) {
        int rand_val = rand() % 500;
        data[i] = rand_val;
    }
    myarg_t args = { num_elements, data };
    myret_t* rvals;
    
    pthread_t p;
    Pthread_create(&p, NULL, mythread, &args);
    Pthread_join(p, (void**) &rvals);
    printf("Average %f\n", rvals->avg);
    free(rvals);
    free(data);
    return 0;
}

