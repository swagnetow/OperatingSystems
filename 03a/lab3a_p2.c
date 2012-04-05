#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>

void add_to_global();

/* Global variable to be added to using threads. */
int global = 0;

/* Global mutex lock. */
pthread_mutex_t lock;

int main() {
    int i;
    int return_value;
    pthread_t tid[10];
    void *arg;

    /* printf("Create threads without sleep().\n"); */
    printf("Create threads with sleep().\n");

    return_value = pthread_mutex_init(&lock, NULL);

    if(return_value < 0) {
        perror("Can't initialize mutex");
        exit(-1);
    }

    for(i = 1; i <= 10; i++) {
        return_value = pthread_create(&tid[i], NULL, (void *)add_to_global, arg);

        if(return_value < 0) {
            perror("Cannot create thread.");
            exit(-1);
        }
    }

    /* Join threads together. */
    for(i = 1; i <= 10; i++) {
        pthread_join(tid[i], NULL);
    }

    return 0;
}

void add_to_global(void* arg) {
    /* Enter critical section. */
    pthread_mutex_lock(&lock);

    int local;

    fprintf(stderr, "Hello, I'm thread %u.\n", (unsigned int)pthread_self());
    local = global;
    sleep(1);

    fprintf(stderr, "Local: %d, TID: %u\n", local, (unsigned int)pthread_self());
    local += 10;
    sleep(1);

    fprintf(stderr, "Local: %d, TID: %u\n", local, (unsigned int)pthread_self());
    global = local;

    /* Exit critical section. Remove this line to cause deadlock. */
    pthread_mutex_unlock(&lock);

    /* Remainder section. */
    sleep(1);
}
