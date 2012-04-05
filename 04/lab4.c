#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int n; /* # of elements for bounded buffer. */
int p; /* # of producer threads. */
int c; /* # of consumer threads. */
int x; /* # of different number that go into the queue. */
int p_time; /* # of cycles to spin-wait between each call to enqueue. */
int c_time; /* # of cycles to spin-wait between each call to dequeue. */
int size; /* Size of the queue. */
int* queue; /* Queue to hold values. */
pthread_mutex_t lock; /* Mutex semaphore. */
sem_t empty; /* Count semaphore increment. */
sem_t full; /* Count semaphore decrement. */

void producer();
void consumer();

int main(int argc, char** argv) {
    int elapsed;
    int i;
    time_t begin;
    time_t end;
    n = atoi(argv[1]);
    p = atoi(argv[2]);
    c = atoi(argv[3]);
    x = atoi(argv[4]);
    p_time = atoi(argv[5]);
    c_time = atoi(argv[6]);
    size = 0;
    queue = malloc(n * sizeof(int));
    pthread_t producers[p];
    pthread_t consumers[c];

    //sem_init(&empty, 0, n);
    //sem_init(&full, 0, 0);
    pthread_mutex_init(&lock, NULL);

    begin = time(NULL);

    printf("%s", asctime(localtime(&begin)));

    /* Create threads. */
    for(i = 0; i < p; i++) {
        pthread_create(&producers[i], NULL, (void *)producer, NULL);
    }

    for(i = 0; i < c; i++) {
        pthread_create(&consumers[i], NULL, (void *)consumer, NULL);
    }

    /* Join threads. */
    for(i = 0; i < p; i++) {
        pthread_join(producers[i], NULL);
    }

    for(i = 0; i < c; i++) {
        pthread_join(consumers[i], NULL);
    }

    end = time(NULL);
    elapsed = end - begin;

    printf("%s", asctime(localtime(&end)));
    printf("Time elapsed: %d seconds.\n", elapsed);

    /* Memory deallocation. */
    free(queue);
    pthread_mutex_destroy(&lock);
    //sem_destroy(&full);
    //sem_destroy(&empty);

    return 0;
}

void producer(void* arg) {
    int i;

    printf("producer:> begin\n");

    do {
        //sem_wait(&empty);
        pthread_mutex_lock(&lock);

        /* Grow queue. */
        for(i = 0; i < p; i++) {
            if(n > size) {
                size += 1;
                queue[size] = rand();
                printf("producer:> enqueue queue[%d] = %d\n", size, queue[size]);
            }
        }

        printf("producer:> size = %d\n", size);

        pthread_mutex_unlock(&lock);
        //sem_post(&full);

        sleep(p_time);
    } while(n > size);

    printf("producer:> end\n");
}

void consumer(void* arg) {
    int i;

    printf("consumer:> begin\n");

    do {
        //sem_wait(&full);
        pthread_mutex_lock(&lock);

        /* Shrink queue. */
        for(i = 0; i < (int)p*(x/c); i++) {
            if(size > 0) {
                size -= 1;
                printf("consumer:> dequeue queue[%d] = %d\n", size, queue[size]);
            }
        }

        printf("consumer:> size = %d\n", size);

        pthread_mutex_unlock(&lock);
        //sem_wait(&empty);

        sleep(c_time);
    } while(size > 0);

    printf("consumer:> end\n");
}
