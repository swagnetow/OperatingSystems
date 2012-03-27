#include <stdio.h>
#include <windows.h>

DWORD WINAPI AddToGlobal(LPVOID);
void CreateThreadsWithoutSleep();
void CreateThreadsWithSleep();

/* Global variable to be added to using threads. */
int global = 0;

CRITICAL_SECTION csl;

int main() {
    int i;
    int param = 0;
    DWORD *tid;
    HANDLE *tid_handle;

    InitializeCriticalSection(&csl);

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

    DeleteCriticalSection(&csl);

    free(tid);
    free(tid_handle);
}

DWORD WINAPI AddToGlobal(LPVOID param) {
    int local;

    /* Start the critical section. */
    EnterCriticalSection(&csl);

    fprintf(stderr, "Hello, I'm thread %u.\n", (unsigned int)GetCurrentThreadId());
    /* Sleep(1000); */
    local = global;

    fprintf(stderr, "Local: %d, TID: %u\n", local, (unsigned int)GetCurrentThreadId());
    /* Sleep(1000); */
    local += 10;

    fprintf(stderr, "Local: %d, TID: %u\n", local, (unsigned int)GetCurrentThreadId());
    global = local;
    /* Sleep(1000); */

    /* End the critical section. Remove this line to cause deadlock. */
    LeaveCriticalSection(&csl);

    return 0;
}
