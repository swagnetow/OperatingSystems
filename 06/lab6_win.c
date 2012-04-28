#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int array_size = 0;
int partitions = 0; /* How to split up the array 2^n times. */
int element = 0;
int num_search_keys = 0; /* Counter for how many times a search key is found. */
int search_key = 0;
int search_key_index = 0; /* The index where the search key is in the array. */
int* array;

DWORD WINAPI SequentialSearch(LPVOID);

int main(int argc, char** argv) {
    float time_elapsed;
    int i;
    int partition_size;
    int return_value;
    int search_key_counter;
    SYSTEMTIME start;
    SYSTEMTIME end;
    DWORD *tid;
    HANDLE *tid_handle;
    array_size = (int) pow(2, atoi(argv[1]));
    array = malloc(sizeof(int) * array_size);
    partitions = (int) pow(2, atoi(argv[2]));
    partition_size = (array_size/partitions);
    tid = (DWORD*)malloc(sizeof(DWORD) * partitions);
    tid_handle = (HANDLE*)malloc(sizeof(DWORD) * partitions);

    /* Start timing the search. */
    GetLocalTime(&start);

    /* Entropy for RNG. */
    srand(time(NULL));

    /* Search key will be a random number between 0-2^n */
    search_key = rand() % array_size;

    for(i = 0; i < array_size; i++) {
        /* Creates random numbers to insert into the array that are between 0-2^n. */
        array[i] = rand() % array_size;
    }

    printf("The search key is: %d\n\n", search_key);

    for(i = 0; i < partitions; i++) {
        /* Array to pass the start and end indicies for splitting up the array into partitions. */
        int *param = (int*)malloc(sizeof(int) * 2);
        param[0] = element;
        element += partition_size;
        param[1] = element;

        tid_handle[i] = CreateThread(NULL, 0, SequentialSearch, &param[0], 0, &tid[i]);
    }

    for(i = 0; i < partitions; i++) {
        WaitForSingleObject(tid_handle[i], INFINITE);
        CloseHandle(tid_handle[i]);
    }

    /* End timing of the search. */
    GetLocalTime(&end);

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

    time_elapsed = (end.wMilliseconds - start.wMilliseconds);

    printf("Time elapsed: %f ms\n", time_elapsed);

    /* Deallocate memory. */
    free(array);
    free(tid);
    free(tid_handle);

    return 0;
}

DWORD WINAPI SequentialSearch(LPVOID param) {
    int i;
    int start;
    int end;
    int *search_args = (int*)malloc(sizeof(int)*2);
    search_args = (int*)param;
    start = search_args[0];
    end = search_args[1];

    /* Search the array sequentially in O(n) time from the split partition of the array. */
    for(i = start; i < end; i++) {
        if(array[i] == search_key) {
            printf("Search key found at element %d!\n", i);
            printf("array[%d] = %d\n\n", i, array[i]);
            search_key_index = i;
            num_search_keys++;
        }
    }

    return 0;
}
