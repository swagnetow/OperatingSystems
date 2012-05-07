#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>

int array_size;
int partitions;
int* array;
int* min;

void selection_sort(void*);
void final_merge();
int get_next();

int main(int argc, char** argv) {
    int i;
    int return_value;
    long seconds;
    long milliseconds;
    long time_elapsed;
    struct timeval start;
    struct timeval end;
    array_size = (int)pow(2, atoi(argv[1]));
    array = malloc(sizeof(int) * array_size);
    min = malloc(sizeof(int) * partitions);
    partitions = (int)pow(2, atoi(argv[2]));
    pthread_t* threads = malloc(sizeof(pthread_t) * partitions);

    printf("array size: %d\n", array_size);
    printf("partitions: %d\n", partitions);
    printf("partition size: %d\n", array_size/partitions);
    printf("\n");

    /* Start timing the search. */
    //gettimeofday(&start, NULL);

    /* Entropy for RNG. */
    srand(time(0));

    for(i = 0; i < array_size; i++) {
        /* Creates random numbers to insert into the array that are between 0-2^n. */
        array[i] = rand() % array_size;
    }

    for(i = 0; i < partitions; i++) {
        return_value = pthread_create(&threads[i], NULL, (void*)selection_sort, (void*)i);

        if(return_value < 0) {
            perror("Cannot create a thread");
            exit(-1);
        }
    }

    for(i = 0; i < partitions; i++) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);

    final_merge();

    seconds = end.tv_sec - start.tv_sec;
    milliseconds = end.tv_usec - start.tv_usec;
    time_elapsed = (((seconds * 1000) + milliseconds)/1000.0) + 0.5;

    printf("Time elapsed: %ld ms\n", time_elapsed);

    free(array);
    free(min);

    return 0;
}

void selection_sort(void *arg) {
    int index = (int)arg;
    int start = (array_size/partitions) * index;
    int end = start + (array_size/partitions) - 1;
    int i;
    int j;
    int minimum;

    for(i = start; i < end; i++) {
        minimum = i;

        for(j = i+1; j < end; j++) {
            if(array[j] < array[minimum]) {
                minimum = j;
            }
        }

        if(minimum != i) {
            int temp;

            temp = array[minimum];
            array[minimum] = array[i];
            array[i] = temp;
        }
    }
}

void final_merge() {
    int i;
    int j;
    int start;
    int end;

    for(i = 0; i < partitions; i++) {
        start = (array_size/partitions) * i;
        end = start + (array_size/partitions);

        printf("partition: %d:\n", i);

        for(j = start; j < end; j++) {
            printf("array[%d] = %d\n", j, array[j]);
        }

        printf("\n\n");
    }

    for(i = 0; i < array_size; i++) {
        int current = get_next();

        printf("array element = %d\n", current);
    }
}

int get_next() {
    int i;
    int smallest;
    int minval = 0;

    for(i = 1; i < partitions; i++) {
        if(array[min[i]] > array[min[minval]]) {
            minval = i;
        }
    }

    smallest = array[min[minval]];
    min[minval]++;

    return smallest;
}
