#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void add_to_global();

/* Global variable to be added to using threads. */
int global = 0;

int main() {
    int i;
    int return_value;
    pthread_t tid[10];
    void *arg;

    printf("Create threads without sleep().\n");

    for(i = 1; i <= 10; i++) {
        return_value = pthread_create(&tid[i], NULL, (void *)add_to_global, arg);

        if(return_value < 0) {
            perror("Cannot create thread.");
            exit(-1);
        }
    }

    for(i = 1; i <= 10; i++) {
        pthread_join(tid[i], NULL);
    }

    return 0;
}

void add_to_global(void* arg) {
    int local;

    fprintf(stderr, "Hello, I'm thread %u.\n", (unsigned int)pthread_self());
    local = global;
    sleep(1);

    fprintf(stderr, "Local: %d, TID: %u\n", local, (unsigned int)pthread_self());
    local += 10;
    sleep(1);

    fprintf(stderr, "Local: %d, TID: %u\n", local, (unsigned int)pthread_self());
    global = local;
    sleep(1);
}
