#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>

void fork_without_sleep();
void fork_with_sleep();

void catcher(int);
void forking(int);

int main(int argc, char** argv) {
    fork_without_sleep();
    fork_with_sleep();

    return 0;
}

void catcher(int sig) {
    printf("Signal catcher called for signal %d.\n", sig);
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
