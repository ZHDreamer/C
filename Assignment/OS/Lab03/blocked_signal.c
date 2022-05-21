#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

void sigchld_handler(int sig) {
    pid_t pid;
    int   status;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("child %d died:%d\n", pid, WEXITSTATUS(status));
        printf("hi, parent process received SIGCHLD signal successfully!\n");
    }
    return;
}
int main() {
    pid_t pid;
    pid = fork();
    if (pid > 0) {
        printf("parent pid = %d\n", getpid());
        signal(SIGCHLD, sigchld_handler);
        pause();
    }
    else if (pid == 0) {
        printf("child pid = %d\n", getpid());
        sleep(3);
        return 1;
    }
    else {
        printf("fork error\n");
    }
    return 0;
}