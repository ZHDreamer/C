#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <wait.h>

void sigint_handler(int sig) {
    printf("received SIGINT signal successed!\n");
    return;
}
int main() {
    pid_t pid;
    pid = fork();
    if (pid > 0) {
        printf("parent pid = %d\n", getpid());
        signal(SIGINT, sigint_handler);
        pause();
    }
    else if (pid == 0) {
        printf("child pid = %d\n", getpid());
        sleep(1);
        printf("after sleep pid = %d\n", getpid());
        sleep(1);
        printf("after the second pid = %d\n", getpid());
    }
    else {
        printf("fork error\n");
    }
    return 0;
}