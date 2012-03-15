#include <stdio.h>
#include <windows.h>

DWORD WINAPI AddToGlobal(LPVOID);
void CreateThreadsWithoutSleep();
void CreateThreadsWithSleep();

/* Global variable to be added to using threads. */
int global = 0;

int main() {
    /* Comment out or uncomment each function and run them one at a time. */
    CreateThreadsWithoutSleep();
    /*
    CreateThreadsWithSleep();
    */
}

DWORD WINAPI AddToGlobal(LPVOID param) {
    int local;

    fprintf(stderr, "Hello, I'm thread %u.\n", (unsigned int)GetCurrentThreadId());
    local = global;

    fprintf(stderr, "Local: %d, TID: %u\n", local, (unsigned int)GetCurrentThreadId());
    local += 10;

    fprintf(stderr, "Local: %d, TID: %u\n", local, (unsigned int)GetCurrentThreadId());
    global = local;

    return 0;
}

void CreateThreadsWithoutSleep() {
    int i;
    int param = 0;
    DWORD *tid;
    HANDLE *tid_handle;

    tid = malloc(sizeof(DWORD) * 10);
    tid_handle = malloc(sizeof(DWORD) * 10);

    for(i = 0; i < 10; i++) {
        tid_handle[i] = CreateThread(NULL, 0, AddToGlobal, &param, 0, &tid[i]);

        if(tid_handle[i] == NULL) {
            fprintf(stderr, "Can't create thread #%d.\n", i+1);
        }
    }

    for(i = 0; i < 10; i++) {
        WaitForSingleObject(tid_handle[i], INFINITE);
        CloseHandle(tid_handle[i]);
    }

    free(tid);
    free(tid_handle);
}

void CreateThreadsWithSleep() {
    int i;
    int param = 0;
    DWORD *tid;
    HANDLE *tid_handle;

    tid = malloc(sizeof(DWORD) * 10);
    tid_handle = malloc(sizeof(DWORD) * 10);

    for(i = 0; i < 10; i++) {
        tid_handle[i] = CreateThread(NULL, 0, AddToGlobal, &param, 0, &tid[i]);

        if(tid_handle[i] == NULL) {
            fprintf(stderr, "Can't create thread #%d.\n", i+1);
        }
    }

    for(i = 0; i < 10; i++) {
        WaitForSingleObject(tid_handle[i], INFINITE);
        Sleep(1);
        CloseHandle(tid_handle[i]);
    }

    free(tid);
    free(tid_handle);
}
