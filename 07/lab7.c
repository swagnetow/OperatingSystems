#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int array_size;
int partitions;
int* array;

void insertion_sort(void*);
void final_merge();

int main(int argc, char** argv) {
    int i;
    int return_value;
    float seconds;
    float nanoseconds;
    float time_elapsed;
    struct timespec start;
    struct timespec end;
    array_size = (int)pow(2, atoi(argv[1]));
    array = malloc(sizeof(int) * array_size);
    partitions = (int)pow(2, atoi(argv[2]));
    pthread_t* threads = malloc(sizeof(pthread_t) * partitions);

    printf("array size: %d\n", array_size);
    printf("partitions: %d\n", partitions);
    printf("partition size: %d\n", array_size/partitions);
    printf("\n");

    /* Entropy for RNG. */
    srand(time(0));

    for(i = 0; i < array_size; i++) {
        /* Creates random numbers to insert into the array that are between 0-2^n. */
        array[i] = (unsigned int)rand() % array_size;
    }

    /* Start timing the search. */
    clock_gettime(CLOCK_REALTIME, &start);

    for(i = 0; i < partitions; i++) {
        return_value = pthread_create(&threads[i], NULL, (void*)insertion_sort, (void*)i);

        if(return_value < 0) {
            perror("Cannot create a thread");
            exit(-1);
        }
    }

    for(i = 0; i < partitions; i++) {
        pthread_join(threads[i], NULL);
    }

    final_merge();

    clock_gettime(CLOCK_REALTIME, &end);

    seconds = end.tv_sec - start.tv_sec;
    nanoseconds = end.tv_nsec - start.tv_nsec;
    time_elapsed = (seconds + nanoseconds)/100000.0;

    printf("Time elapsed: %lf ns\n", time_elapsed);

    free(array);

    return 0;
}

void insertion_sort(void *arg) {
    int index = (int)arg;
    int start = (array_size/partitions) * index;
    int end = start + (array_size/partitions) - 1;
    int i, j, value;

    for(i = start; i <= end; i++) {
        value = array[i];

        for(j = i; j > 0 && value < array[j-1]; j--) {
            array[j] = array[j-1];
        }

        array[j] = value;
    }

    /*
    for(i = start; i <=end; i++) {
        printf("array[%d] = %d\n", i, array[i]);
    }

    printf("\n");
    */
}

void final_merge() {
    int i;
    int j;
    int value;

    for(i = 0; i < array_size; i++) {
        value = array[i];

        for(j = i; j > 0 && value < array[j-1]; j--) {
            array[j] = array[j-1];
        }

        array[j] = value;
    }

    /*
    for(i = 0; i < array_size; i++) {
        printf("%d\n", array[i]);
    }
    */
}
