#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int array_size = 0;
int element = 0;
int num_search_keys = 0; /* Counter for how many times a search key is found. */
int search_key = 0;
int search_key_index = 0; /* The index where the search key is in the array. */
int partitions = 0; /* How to split up the array 2^n times. */
int* array;

void sequential_search(void*);

int main(int argc, char** argv) {
    array_size = (int)pow(2, atoi(argv[1]));
    array = malloc(sizeof(int) * array_size);
    partitions = (int)pow(2, atoi(argv[2]));
    pthread_t* threads = malloc(sizeof(pthread_t) * partitions);
    int i;
    int partition_size = (array_size/partitions);
    int return_value;
    int search_key_counter;
    long seconds;
    long milliseconds;
    long time_elapsed;
    struct timeval start;
    struct timeval end;

    /* Start timing the search. */
    gettimeofday(&start, NULL);

    /* Entropy for RNG. */
    srand(time(0));

    /* Search key will be a random number between 0-2^n */
    search_key = rand() % array_size;

    for(i = 0; i < array_size; i++) {
        /* Creates random numbers to insert into the array that are between 0-2^n. */
        array[i] = rand() % array_size;
    }

    printf("The search key is: %d\n\n", search_key);

    for(i = 0; i < partitions; i++) {
        /* Array to pass the start and end indicies for splitting up the array into partitions. */
        int *thread_args = malloc(sizeof(int) * 2);

        thread_args[0] = element;
        element += partition_size;
        thread_args[1] = element;

        return_value = pthread_create(&threads[i], NULL, (void *)sequential_search, (void *)thread_args);

        if(return_value < 0) {
            perror("Cannot create a thread");
            exit(-1);
        }
    }

    for(i = 0; i < partitions; i++) {
        pthread_join(threads[i], NULL);
    }

    /* End timing of the search. */
    gettimeofday(&end, NULL);

    /* Final check for the search key within the array. */
    for(i = 0; i < array_size; i++) {
        if(array[i] == search_key) {
            printf("Search key found at element %d in the final search!\n", i);
            printf("array[%d] = %d\n\n", i, array[i]);
        }
    }

    /* Displays and error if the search key is found more than once. */
    if(num_search_keys > 1) {
        printf("There was an error in the search and the search key was found in the array more than once!\n\n");
    }
    else if(num_search_keys == 0) {
        printf("The search key was not found!\n\n");
    }

    seconds = end.tv_sec - start.tv_sec;
    milliseconds = end.tv_usec - start.tv_usec;
    time_elapsed = (((seconds * 1000) + milliseconds)/1000.0) + 0.5;

    printf("Time elapsed: %ld ms\n", time_elapsed);

    free(array);

    return 0;
}

void sequential_search(void* args) {
    int* search_args = (int*)args;
    int start = search_args[0];
    int end = search_args[1];
    int i;

    /* Search the array sequentially in O(n) time from the split partition of the array. */
    for(i = start; i < end; i++) {
        if(array[i] == search_key) {
            printf("Search key found at element %d!\n", i);
            printf("array[%d] = %d\n\n", i, array[i]);
            search_key_index = i;
            num_search_keys++;
        }
    }
}
