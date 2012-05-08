#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

int array_size;
int partitions;
int* array;

DWORD WINAPI InsertionSort(LPVOID);
void final_merge();

int main(int argc, char** argv) {
    float time_elapsed;
    int i;
    int return_value;
    SYSTEMTIME start;
    SYSTEMTIME end;
    DWORD *tid;
    HANDLE *tid_handle;
    array_size = (int)pow(2, atoi(argv[1]));
    array = malloc(sizeof(int) * array_size);
    partitions = (int)pow(2, atoi(argv[2]));
    tid = (DWORD*)malloc(sizeof(DWORD) * partitions);
    tid_handle = (HANDLE*)malloc(sizeof(DWORD) * partitions);

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
    GetLocalTime(&start);

    for(i = 0; i < partitions; i++) {
        tid_handle[i] = CreateThread(NULL, 0, InsertionSort, (LPVOID)i, 0, &tid[i]);
    }

    for(i = 0; i < partitions; i++) {
        WaitForSingleObject(tid_handle[i], INFINITE);
        CloseHandle(tid_handle[i]);
    }

    GetLocalTime(&end);

    final_merge();

    time_elapsed = (end.wMilliseconds - start.wMilliseconds);

    printf("Time elapsed: %lf ms\n", time_elapsed);

    free(array);
    free(tid);
    free(tid_handle);

    return 0;
}

DWORD WINAPI InsertionSort(LPVOID param) {
    int index = (int)param;
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

    return 0;
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
