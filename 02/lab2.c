#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pid;
    int bg;
    char* token;
    char* argv[16];
    char buffer[64];

    while(1) {
        int i = 0;

        /* L O S T shell prompt. */
        write(1, ":> ", 3);
        read(0, buffer, sizeof(buffer));

        /* Initialize tokenization. */
        token = strtok(buffer, " \n");

        /* Tokenize the input to be put into a pointer to a char array. */
        while(token != NULL) {
            argv[i++] = token;
            token = strtok(NULL, " \n");
        }

        bg = 0;

        /* Set flag if the process should be run in the background. */
        if(strcmp(argv[i-1], "&") == 0) {
            bg = 1;
            argv[i-1] = NULL;
        }

        /* Sets the NULL termination character */
        argv[i] = NULL;

        pid = fork();

        if(pid == 0) {
            /* Execute the command. */
            execvp(argv[0], &argv[0]);
        }
        else {
            /* Checks if process is in the background. */
            if(bg == 0) {
                /* Returns to parent. */
                waitpid(pid, 0, 0);
            }
        }
    }

    return 0;
}
