#include <stdio.h>
#include <windows.h>

int main() {
    int pid;
    int bg;
    char buffer[64];
    char program[32];
    char* line;
    char* tokens[64];
    HANDLE UserInput = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE ScreenOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD read, write;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    HANDLE ReadHandle, WriteHandle;
    SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};

    while(1) {
        int i = 0;
        int k;

        memset(&buffer, 0, sizeof(buffer));
        memset(&program, 0, sizeof(program));

        /* L O S T shell prompt. */
        WriteFile(ScreenOutput, ">: ", 3, &write, NULL);

        /* Take user input. */
        ReadFile(UserInput, &buffer, sizeof(buffer), &read, NULL);

        /* Tokenize the input to be put into a pointer to a char array. */
        line = strtok(buffer, " \n");

        while(line != NULL) {
            tokens[i++] = line;
            line = strtok(NULL, " \n");
        }

        sprintf(program, "%s", tokens[0]);

        bg = 0;

        /* Check if the program is going to be run in the background. */
        for(k = 0; k < sizeof(buffer); k++) {
            if(buffer[k] == 38) {
                bg = 1;
                break;
            }
        }

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        /* Execute if not in the background. */
        if(bg == 0) {
            if(!CreateProcess(NULL, program, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
		        fprintf(stderr, "Create Process Failed\n");
		        return -1;
            }

            /* Returns to the parent process. */
	        WaitForSingleObject(pi.hProcess, INFINITE);
        }
        else {
            /* Runs process in the background. */
            si.hStdOutput = NULL;

            if(!CreateProcess(NULL, program, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
		        fprintf(stderr, "Create Process Failed\n");
		        return -1;
            }
        }

        /* Close handles. */
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
	}

    return 0;
}
