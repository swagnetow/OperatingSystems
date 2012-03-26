#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>

void fork_without_sleep();
void fork_with_sleep();
void fork_using_one_fprintf();
void fork_using_fputs();
void fork_using_write();

void catcher(int);
void forking(int);

int main(int argc, char** argv) {
    int type;

    type = atoi(argv[1]);

    /* Uncomment out to see how switch statements interact with synchronization. */
    /* forking(type); */

    /* Comment out when forking() is uncommented. */
    switch(type) {
        case 1:
            printf("Part 1 using fprintf() without sleep():\n");
            fork_without_sleep();
            break;
        case 2:
            printf("Part 1 using fprintf() with sleep():\n");
            fork_with_sleep();
            break;
        case 3:
            printf("Part 1 using one fprintf():\n");
            fork_using_one_fprintf();
            break;
        case 4:
            printf("Part 1 using fputs():\n");
            fork_using_fputs();
            break;
        case 5:
            printf("Part 1 using write():\n");
            fork_using_write();
            break;
        default:
            printf("Incorrect option!\n");
            break;
    }

    return 0;
}

void catcher(int sig) {
    printf("Signal catcher called for signal %d.\n", sig);
}

void forking(int num) {
    char buffer[4096];
    int i;
    int proc;
    int result = 0;
    int signal;
    int parent_pid;
    int type;
    pid_t child_pid;
    sigset_t waitset;
    struct sigaction sigact;

    /* Set signal code. */
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigact.sa_handler = catcher;
    sigaction(SIGUSR1, &sigact, NULL);
    sigemptyset(&waitset);
    sigaddset(&waitset, SIGUSR1);
    sigprocmask(SIG_BLOCK, &waitset, NULL);

    parent_pid = getpid();
    type = num;

    for(proc = 0; proc < 10; proc++) {
        child_pid = fork();

        /* Check if program should sleep. */
        if(type == 2) {
            sleep(1);
        }

        if(child_pid) {
            break;
        }
    }

    switch(type) {
        case 1:
            for(i = 0; i < 10; i++ ) {
                fprintf(stderr, "Process: %d, PID: %d\n", i+1, getpid());
            }

            break;
        case 2:
            for(i = 0; i < 10; i++ ) {
                fprintf(stderr, "Process: %d, PID: %d\n", i+1, getpid());
            }

            break;
        case 3:
            memset(buffer, 0, sizeof(buffer));

            for(i = 0; i < 10; i++ ) {
                if(i == 0) {
                    sprintf(buffer, "Process: %d, PID: %d\n", i+1, getpid());
                }
                else {
                    sprintf(buffer, "%sProcess: %d, PID: %d\n", buffer, i+1, getpid());
                }
            }

            fprintf(stderr, "%s", buffer);
            break;
        case 4:
            memset(buffer, 0, sizeof(buffer));

            for(i = 0; i < 10; i++ ) {
                sprintf(buffer, "Process: %d, PID: %d\n\0", i+1, getpid());
                fputs(buffer, stderr);
            }

            break;
        case 5:
            memset(buffer, 0, sizeof(buffer));

            for(i = 0; i < 10; i++ ) {
                if(i == 0) {
                    sprintf(buffer, "Process: %d, PID: %d\n", i+1, getpid());
                }
                else {
                    sprintf(buffer, "%sProcess: %d, PID: %d\n", buffer, i+1, getpid());
                }
            }

            write(2, buffer, sizeof(buffer));
            break;
        default:
            printf("Incorrect option!\n");
            break;
    }

    if(parent_pid != getpid()) {
        result = sigwait(&waitset, &signal);

        if(result == 0) {
            printf("PID: %d, sigwait() returned for signal %d.\n", getpid(), signal);
        }
        else {
            printf("sigwait() returned error number %d.\n", errno);
            perror("sigwait() function failed.\n");
        }
    }

    if(kill(child_pid, SIGUSR1) < 0) {
        perror("Failed to kill child");
    }

    wait(child_pid);
}

void fork_without_sleep() {
    int i;
    int proc;
    int result = 0;
    int signal;
    int sig_pid;
    pid_t child_pid;
    sigset_t waitset;
    struct sigaction sigact;

    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigact.sa_handler = catcher;
    sigaction(SIGUSR1, &sigact, NULL);
    sigemptyset(&waitset);
    sigaddset(&waitset, SIGUSR1);
    sigprocmask(SIG_BLOCK, &waitset, NULL);

    sig_pid = getpid();

    printf("Part 1 without sleep():\n");

    for(proc = 0; proc < 10; proc++) {
        child_pid = fork();

        if(child_pid) {
            break;
        }
    }

    if(sig_pid != getpid()) {
        result = sigwait(&waitset, &signal);

        if(result == 0) {
            printf("PID: %d, sigwait() returned for signal %d.\n", getpid(), signal);
        }
        else {
            printf("sigwait() returned error number %d.\n", errno);
            perror("sigwait() function failed.\n");
        }
    }

    for(i = 0; i < 10; i++ ) {
        fprintf(stderr, "Process: %d, PID: %d\n", i+1, getpid());
    }

    if(kill(child_pid, SIGUSR1) < 0) {
        perror("Failed to kill child");
    }

    wait(child_pid);
}

void fork_with_sleep() {
    int i;
    int proc;
    int result = 0;
    int signal;
    int sig_pid;
    pid_t child_pid;
    sigset_t waitset;
    struct sigaction sigact;

    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigact.sa_handler = catcher;
    sigaction(SIGUSR1, &sigact, NULL);
    sigemptyset(&waitset);
    sigaddset(&waitset, SIGUSR1);
    sigprocmask(SIG_BLOCK, &waitset, NULL);

    sig_pid = getpid();

    printf("Part 1 with sleep():\n");

    for(proc = 0; proc < 10; proc++) {
        child_pid = fork();

        if(child_pid) {
            break;
        }

        sleep(1);
    }

    if(sig_pid != getpid()) {
        result = sigwait(&waitset, &signal);

        if(result == 0) {
            printf("PID: %d, sigwait() returned for signal %d.\n", getpid(), signal);
        }
        else {
            printf("sigwait() returned error number %d.\n", errno);
            perror("sigwait() function failed.\n");
        }
    }

    for(i = 0; i < 10; i++ ) {
        fprintf(stderr, "Process: %d, PID: %d\n", i+1, getpid());
    }

    if(kill(child_pid, SIGUSR1) < 0) {
        perror("Failed to kill child");
    }

    wait(child_pid);
}

void fork_using_one_fprintf() {
    char buffer[2048];
    int i;
    int proc;
    int result = 0;
    int signal;
    int sig_pid;
    pid_t child_pid;
    sigset_t waitset;
    struct sigaction sigact;

    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigact.sa_handler = catcher;
    sigaction(SIGUSR1, &sigact, NULL);
    sigemptyset(&waitset);
    sigaddset(&waitset, SIGUSR1);
    sigprocmask(SIG_BLOCK, &waitset, NULL);

    sig_pid = getpid();

    printf("Part 1 using one fprintf():\n");

    for(proc = 0; proc < 10; proc++) {
        child_pid = fork();

        if(child_pid) {
            break;
        }
    }

    if(sig_pid != getpid()) {
        result = sigwait(&waitset, &signal);

        if(result == 0) {
            printf("PID: %d, sigwait() returned for signal %d.\n", getpid(), signal);
        }
        else {
            printf("sigwait() returned error number %d.\n", errno);
            perror("sigwait() function failed.\n");
        }
    }

    memset(buffer, 0, sizeof(buffer));

    for(i = 0; i < 10; i++ ) {
        if(i == 0) {
            sprintf(buffer, "Process: %d, PID: %d\n", i+1, getpid());
        }
        else {
            sprintf(buffer, "%sProcess: %d, PID: %d\n", buffer, i+1, getpid());
        }
    }

    fprintf(stderr, "%s", buffer);

    if(kill(child_pid, SIGUSR1) < 0) {
        perror("Failed to kill child");
    }

    wait(child_pid);
}

void fork_using_fputs() {
    char buffer[32];
    int i;
    int proc;
    int result = 0;
    int signal;
    int sig_pid;
    pid_t child_pid;
    sigset_t waitset;
    struct sigaction sigact;

    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigact.sa_handler = catcher;
    sigaction(SIGUSR1, &sigact, NULL);
    sigemptyset(&waitset);
    sigaddset(&waitset, SIGUSR1);
    sigprocmask(SIG_BLOCK, &waitset, NULL);

    sig_pid = getpid();

    printf("Part 1 using fputs():\n");

    for(proc = 0; proc < 10; proc++) {
        child_pid = fork();

        if(child_pid) {
            break;
        }
    }

    if(sig_pid != getpid()) {
        result = sigwait(&waitset, &signal);

        if(result == 0) {
            printf("PID: %d, sigwait() returned for signal %d.\n", getpid(), signal);
        }
        else {
            printf("sigwait() returned error number %d.\n", errno);
            perror("sigwait() function failed.\n");
        }
    }

    memset(buffer, 0, sizeof(buffer));

    for(i = 0; i < 10; i++ ) {
        sprintf(buffer, "Process: %d, PID: %d\n\0", i+1, getpid());
        fputs(buffer, stderr);
    }

    if(kill(child_pid, SIGUSR1) < 0) {
        perror("Failed to kill child");
    }

    wait(child_pid);
}

void fork_using_write() {
    char buffer[4096];
    int i;
    int proc;
    int result = 0;
    int signal;
    int sig_pid;
    pid_t child_pid;
    sigset_t waitset;
    struct sigaction sigact;

    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigact.sa_handler = catcher;
    sigaction(SIGUSR1, &sigact, NULL);
    sigemptyset(&waitset);
    sigaddset(&waitset, SIGUSR1);
    sigprocmask(SIG_BLOCK, &waitset, NULL);

    sig_pid = getpid();

    printf("Part 1 using write():\n");

    for(proc = 0; proc < 10; proc++) {
        child_pid = fork();

        if(child_pid) {
            break;
        }
    }

    if(sig_pid != getpid()) {
        result = sigwait(&waitset, &signal);

        if(result == 0) {
            printf("PID: %d, sigwait() returned for signal %d.\n", getpid(), signal);
        }
        else {
            printf("sigwait() returned error number %d.\n", errno);
            perror("sigwait() function failed.\n");
        }
    }

    memset(buffer, 0, sizeof(buffer));

    for(i = 0; i < 10; i++ ) {
        if(i == 0) {
            sprintf(buffer, "Process: %d, PID: %d\n", i+1, getpid());
        }
        else {
            sprintf(buffer, "%sProcess: %d, PID: %d\n", buffer, i+1, getpid());
        }
    }

    write(2, buffer, sizeof(buffer));

    if(kill(child_pid, SIGUSR1) < 0) {
        perror("Failed to kill child");
    }

    wait(child_pid);
}
