#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int search_key;
int search_key_index;
int* array;
int array_size;
int partitions;

void sequential_search(void*);

int main(int argc, char** argv) {
    array_size = (int)pow(2, atoi(argv[1]));
    partitions = (int)pow(2, atoi(argv[2]));
    int pivot = 1;
    int i;
    int *thread_args = malloc(sizeof(int) * 2);
    int return_value;
    int search_key_counter;
    pthread_t* search_threads = malloc(sizeof(pthread_t) * partitions);
    array = malloc(sizeof(int) * array_size);
    search_key = atoi(argv[3]);

    srand(time(0));

    for(i = 0; i < array_size; i++) {
        array[i] = rand() % array_size;
    }

    for(i = 0; i < partitions; i++) {
        printf("%d to %d!\n", pivot, pivot+partitions);
        thread_args[0] = pivot;
        pivot += partitions;
        thread_args[1] = pivot;
        return_value = pthread_create(&search_threads[i], NULL, (void *)sequential_search, (void *)thread_args);

        if(return_value < 0) {
            perror("Cannot create a thread");
            exit(-1);
        }
    }

    for(i = 0; i < partitions; i++) {
        pthread_join(search_threads[i], NULL);
    }

    for(i = 0; i < array_size; i++) {
        if(array[i] == search_key) {
            printf("Search key found at element %d!\n", i);
        }
    }

    free(array);

    return 0;
}

void sequential_search(void* args) {
    int* search_args = (int*)args;
    int start = search_args[0];
    int end = search_args[1];
    int i;

    printf("%d to %d\n", start, end);

    for(i = start; i < end; i++) {
        if(array[i] == search_key) {
            printf("Search key found at element %d!\n", i);
            search_key_index = i;
        }
    }
}
