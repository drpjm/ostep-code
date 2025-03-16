#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "common_threads.h"

typedef struct {
    long long int value;
    int num_iters;
    pthread_mutex_t lock;
} loop_args_t;

void *mythread(void *arg) {
    loop_args_t* loop_args = (loop_args_t*) arg;
    for (int i = 0; i < loop_args->num_iters; i++) {
        pthread_mutex_lock(&loop_args->lock);
        loop_args->value += 4;
        pthread_mutex_unlock(&loop_args->lock);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    // Get input for number of loop iterations
    if (argc != 2) {
        printf("Error: need the number of loops to run.\n");
        exit(EXIT_FAILURE);
    }

    int num_iters = atoi(argv[1]);
    loop_args_t* loop_args = malloc(sizeof(loop_args_t));
    loop_args->num_iters = num_iters;
    loop_args->value = 0;
    int rc = pthread_mutex_init(&loop_args->lock, NULL);
    if (rc != 0) {
        printf("Failed to create a lock.\n");
        exit(EXIT_FAILURE);
    }
    pthread_t p;
    Pthread_create(&p, NULL, mythread, (void *) loop_args);
    for (int i = 0; i < num_iters; i++) {
        pthread_mutex_lock(&loop_args->lock);
        loop_args->value += 2;
        pthread_mutex_unlock(&loop_args->lock);
    }
    Pthread_join(p, NULL);
    printf("returned %lld\n", loop_args->value);
    return 0;
}

