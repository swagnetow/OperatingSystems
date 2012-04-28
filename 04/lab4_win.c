#include <stdio.h>
#include <time.h>
#include <windows.h>

int n; /* # of elements for bounded buffer. */
int p; /* # of producer threads. */
int c; /* # of consumer threads. */
int x; /* # of different number that go into the queue. */
int p_time; /* # of cycles to spin-wait between each call to enqueue. */
int c_time; /* # of cycles to spin-wait between each call to dequeue. */
int size; /* Size of the queue. */
int* queue; /* Queue to hold values. */
HANDLE lock; /* Mutex semaphore. */
HANDLE empty; /* Count semaphore decrement. */
HANDLE full; /* Count semaphore increment. */

DWORD WINAPI producer();
DWORD WINAPI consumer();

int main(int argc, char** argv) {
    int elapsed;
    int i;
    time_t begin;
    time_t end;
    HANDLE *producers;
    HANDLE *consumers;
    n = atoi(argv[1]);
    p = atoi(argv[2]);
    c = atoi(argv[3]);
    x = atoi(argv[4]);
    p_time = atoi(argv[5]);
    c_time = atoi(argv[6]);
    size = 0;
    queue = malloc(n * sizeof(int));
    empty = CreateSemaphore(NULL, 0, 0, NULL);
    full = CreateSemaphore(NULL, size, size, NULL);
    lock = CreateMutex(NULL, FALSE, NULL);
    producers = malloc(p * sizeof(HANDLE));
    consumers = malloc(c * sizeof(HANDLE));

    begin = time(NULL);

    /* Beginning timestamp. */
    printf("%s\n", asctime(localtime(&begin)));

    /* Create threads. */
    for(i = 0; i < p; i++) {
        producers[i] = CreateThread(NULL, 0, (void*)producer, NULL, 0, NULL);
    }

    for(i = 0; i < c; i++) {
        consumers[i] = CreateThread(NULL, 0, (void*)producer, NULL, 0, NULL);
    }

    /* Join threads. */
    for(i = 0; i < p; i++) {
        WaitForSingleObject(producers[i], INFINITE);
        CloseHandle(producers[i]);
    }

    for(i = 0; i < c; i++) {
        WaitForSingleObject(consumers[i], INFINITE);
        CloseHandle(consumers[i]);
    }

    end = time(NULL);
    elapsed = end - begin;

    /* Ending timestamp. */
    printf("\n%s", asctime(localtime(&end)));
    printf("Time elapsed: %d seconds.\n", elapsed);

    /* Memory deallocation. */
    CloseHandle(empty);
    CloseHandle(full);
    CloseHandle(lock);
    free(queue);
    free(producers);
    free(consumers);

    return 0;
}

DWORD WINAPI producer() {
    int i;

    printf("producer:> begin\n");

    do {
        /* P() */
        WaitForSingleObject(empty, 0);
        WaitForSingleObject(lock, INFINITE);

        /* Induce race conditions. */
        Sleep(1);

        /* Grow queue. */
        for(i = 0; i < p; i++) {
            if(n > size) {
                size += 1;
                queue[size-1] = rand();
                printf("producer:> enqueue queue[%d] = %d\n", size, queue[size-1]);
            }
        }

        printf("producer:> size = %d\n", size);

        /* V() */
        ReleaseMutex(lock);
        ReleaseSemaphore(full, p, NULL);

        Sleep(p_time);
    } while(n > size);

    printf("producer:> end\n");

    return 0;
}

DWORD WINAPI consumer() {
    int i;

    printf("consumer:> begin\n");

    do {
        /* P() */
        WaitForSingleObject(full, 0);
        WaitForSingleObject(lock, INFINITE);

        /* Induce race conditions. */
        Sleep(1);

        /* Shrink queue. */
        for(i = 0; i < (int)p*(x/c); i++) {
            if(size > 0) {
                printf("consumer:> dequeue queue[%d] = %d\n", size-1, queue[size-1]);
                size -= 1;
            }
        }

        printf("consumer:> size = %d\n", size);

        /* V() */
        ReleaseMutex(lock);
        ReleaseSemaphore(empty, (int)p*(x/c), NULL);

        Sleep(c_time);
    } while(size > 0);

    printf("consumer:> end\n");

    return 0;
}
