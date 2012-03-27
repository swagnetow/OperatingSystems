#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void CreateProcessWithSleep();
void CreateProcessWithoutSleep();
void CreateProcessUsingOthers();

int main() {
    /* Comment out or uncomment each function and run them one at a time. */
    /*
    CreateProcessWithoutSleep();
    CreateProcessWithSleep();
    CreateProcessUsingOthers();
    */

    return 0;
}

void CreateProcessWithoutSleep() {
    int i;
    STARTUPINFO si[10];
    PROCESS_INFORMATION pi[10];

    printf("Part 1 without Sleep():\n");

    for(i = 0; i < 10; i++) {
        ZeroMemory(&si[i], sizeof(si[i]));
        si[i].cb = sizeof(si[i]);
        ZeroMemory(&pi[i], sizeof(pi[i]));
        GetStartupInfo(&si[i]);

        if(!CreateProcess(NULL, "proc.exe", NULL, NULL, TRUE, 0, NULL, NULL, &si[i], &pi[i])) {
		    fprintf(stderr, "Create Process Failed\n");
            exit(-1);
        }
    }

    for(i = 0; i < 10; i++) {
        WaitForSingleObject(pi[i].hProcess, INFINITE);
    }
}

void CreateProcessWithSleep() {
    int i;
    STARTUPINFO si[10];
    PROCESS_INFORMATION pi[10];

    printf("Part 1 with Sleep():\n");

    for(i = 0; i < 10; i++) {
        ZeroMemory(&si[i], sizeof(si[i]));
        si[i].cb = sizeof(si[i]);
        ZeroMemory(&pi[i], sizeof(pi[i]));
        GetStartupInfo(&si[i]);

        if(!CreateProcess(NULL, "proc.exe", NULL, NULL, TRUE, 0, NULL, NULL, &si[i], &pi[i])) {
		    fprintf(stderr, "Create Process Failed\n");
            exit(-1);
        }

        Sleep(1);
    }

    for(i = 0; i < 10; i++) {
        WaitForSingleObject(pi[i].hProcess, INFINITE);
    }
}

void CreateProcessUsingOthers() {
    int i;
    STARTUPINFO si[10];
    PROCESS_INFORMATION pi[10];

    printf("Part 1 using one fprintf()/fputs()/WriteFile():\n");

    for(i = 0; i < 10; i++) {
        ZeroMemory(&si[i], sizeof(si[i]));
        si[i].cb = sizeof(si[i]);
        ZeroMemory(&pi[i], sizeof(pi[i]));
        GetStartupInfo(&si[i]);

        if(!CreateProcess(NULL, "proc2.exe", NULL, NULL, TRUE, 0, NULL, NULL, &si[i], &pi[i])) {
		    fprintf(stderr, "Create Process Failed\n");
            exit(-1);
        }
    }

    for(i = 0; i < 10; i++) {
        WaitForSingleObject(pi[i].hProcess, INFINITE);
    }
}
