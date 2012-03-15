#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fork_without_sleep();
void fork_with_sleep();
void fork_using_one_fprintf();
void fork_using_fputs();
void fork_using_write();

int main() {
    /* Comment out or uncomment each function and run them one at a time. */
    fork_without_sleep();
    /*
    fork_with_sleep();
    fork_using_one_fprintf();
    fork_using_fputs();
    fork_using_write();
    */

    return 0;
}

void fork_without_sleep() {
    int i;
    int pid[10];
    int proc;

    printf("Part 1 without sleep():\n");

    for(proc = 0; proc < 10; proc++) {
        pid[i] = fork();

        if(pid[i] == 0) {
            for(i = 0; i < 10; i++) {
                fprintf(stderr, "Process: %d, PID: %d\n", i+1, getpid());
            }

            exit(0);
        }
        else if(pid[i] == -1) {
            fprintf(stderr, "Failed to create child for PID!\n");
        }
    }

    for(i = 0; i < 10; i++) {
        waitpid(pid[i], 0, 0);
    }
}

void fork_with_sleep() {
    int i;
    int pid[i];
    int proc;

    printf("Part 1 with sleep():\n");

    for(proc = 0; proc < 10; proc++) {
        pid[i] = fork();

        if(pid[i] == 0) {
            for(i = 0; i < 10; i++ ) {
                fprintf(stderr, "Process: %d, PID: %d\n", i+1, getpid());
            }

            sleep(1);
            exit(0);
        }
        else if(pid[0] == -1) {
            fprintf(stderr, "Failed to create child for PID ", proc);
        }
    }

    for(i = 0; i < 10; i++) {
        waitpid(pid[i], 0, 0);
    }
}

void fork_using_one_fprintf() {
    char buffer[2048];
    int i;
    int pid[i];
    int proc;

    printf("Part 1 using one fprintf():\n");

    for(proc = 0; proc < 10; proc++) {
        memset(buffer, 0, sizeof(buffer));

        pid[i] = fork();

        if(pid[i] == 0) {
            for(i = 0; i < 10; i++) {
                if(proc == 0) {
                    sprintf(buffer, "Process: %d, PID: %d\n", i+1, getpid());
                }
                else {
                    sprintf(buffer, "%sProcess: %d, PID: %d\n", buffer, i+1, getpid());
                }
            }

            fprintf(stderr, "%s", buffer);

            exit(0);
        }
        else if(pid[i] == -1) {
            fprintf(stderr, "Failed to create child for PID ", proc);
        }
    }

    for(i = 0; i < 10; i++) {
        waitpid(pid[i], 0, 0);
    }

}

void fork_using_fputs() {
    char buffer[32];
    int i;
    int pid[10];
    int proc;

    printf("Part 1 using fputs():\n");

    memset(buffer, 0, sizeof(buffer));

    for(proc = 0; proc < 10; proc++) {
        pid[i] = fork();

        if(pid[i] == 0) {
            for(i = 0; i < 10; i++) {
                sprintf(buffer, "Process: %d, PID: %d\n\0", i, getpid());
                fputs(buffer, stderr);
            }
        }
        else if(pid[i] == -1) {
            fprintf(stderr, "Failed to create child for PID ", proc);
        }
    }
}

void fork_using_write() {
    char buffer[4096];
    int i;
    int pid[10];
    int proc;

    printf("Part 1 using write():\n");

    memset(buffer, 0, sizeof(buffer));

    for(proc = 0; proc < 10; proc++) {
        pid[i] = fork();

        if(pid[i] == 0) {
            for(i = 0; i < 10; i++) {
                if(proc == 0) {
                    sprintf(buffer, "Process: %d, PID: %d\n", i+1, getpid());
                }
                else {
                    sprintf(buffer, "%sProcess: %d, PID: %d\n", buffer, i+1, getpid());
                }
            }

            exit(0);
        }
        else if(pid[i] == -1) {
            fprintf(stderr, "Failed to create child for PID ", proc);
        }
    }

    write(2, buffer, sizeof(buffer));
}
