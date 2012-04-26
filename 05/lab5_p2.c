#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 100
#define FRAME_SIZE 2

int head = 0;
int fifo_page_faults = 0;
int lru_page_faults = 0;

struct Page {
    int index;
    int reference;
};

void fifo(int*, int);
int frame_exists(int*, int);
void lru(struct Page *, int, int);
void opt(int*, int);
int frame_exists_lru(struct Page *, int, int);
int lru_lookup(struct Page *);

int main() {
    int i;
    int page_frames[FRAME_SIZE];
    int page_reference[SIZE];
    struct Page page_frames_lru[FRAME_SIZE];

    /* Generates entropy for rand(). */
    srand(time(0));

    for(i = 0; i < SIZE; i++) {
        page_reference[i] = rand() % 14;
        printf("%d ", page_reference[i]);
    }
    printf("\n");

    for(i = 0; i < FRAME_SIZE; i++) {
        page_frames[i] = -1;
    }

    for(i = 0; i < SIZE; i++) {
        fifo(page_frames, page_reference[i]);
    }

    printf("FIFO page faults: %d\n", fifo_page_faults);

    for(i = 0; i < FRAME_SIZE; i++) {
        page_frames_lru[i].index = -1;
        page_frames_lru[i].reference = -1;
    }

    for(i = 0; i < SIZE; i++) {
        lru(page_frames_lru, page_reference[i], i);
    }

    printf("LRU page faults: %d\n", lru_page_faults);

    return 0;
}

void fifo(int* page_frames, int page_number) {
    int current_page_number;

    if(!frame_exists(page_frames, page_number)) {
        current_page_number = page_frames[head];
        page_frames[head] = page_number;
        head = (head + 1) % FRAME_SIZE;
        fifo_page_faults++;
    }
}

int frame_exists(int* page_frames, int page_number) {
    int i;

    for(i = 0; i < FRAME_SIZE; i++) {
        if(page_frames[i] == page_number) {
            return 1;
        }
    }

    return 0;
}

void lru(struct Page *page_frames_lru, int page_reference, int index) {
    int current_index;
    int current_page_reference;

    if(!frame_exists_lru(page_frames_lru, page_reference, index)) {
        current_index = lru_lookup(page_frames_lru);
        current_page_reference = page_frames_lru[current_index].reference;
        page_frames_lru[current_index].reference = page_reference;
        page_frames_lru[current_index].index = index;
        lru_page_faults++;
    }
}

int frame_exists_lru(struct Page *page_frames_lru, int page_number, int index) {
    int i;

    for(i = 0; i < FRAME_SIZE; i++) {
        if(page_frames_lru[i].reference == page_number) {
            page_frames_lru[i].index = index;
            return 1;
        }
    }

    return 0;
}

int lru_lookup(struct Page *page_frames_lru) {
    int i;
    int k = 0;
    int min = -1;

    for(i = 1; i < SIZE; i++) {
        if(min > page_frames_lru[i].index) {
            min = page_frames_lru[i].index;
            k = i;
        }
    }

    return k;
}

void opt(int* page_frames, int page_number) {
}
