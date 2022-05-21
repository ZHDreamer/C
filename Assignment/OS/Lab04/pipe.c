#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main() {
    int fd[2];
    if (pipe(fd) == 0) {
        pid_t pid;
        pid = fork();
        if (pid > 0) {
            char buf[] = "pipe test";
            close(fd[0]);
            write(fd[1], buf, 10 * sizeof(char));
            printf("parent write success!\n");
            close(fd[1]);
            printf("parent close pipe success!\n");
            waitpid(-1, NULL, 0);
        }
        else if (pid == 0) {
            close(fd[1]);
            sleep(1);
            char buf[10];
            read(fd[0], buf, 10 * sizeof(char));
            printf("read data from pipe successed!\n");
            printf("the data:%s\n", buf);
            close(fd[0]);
        }
        else {
            printf("fork error\n");
        }
        return 0;
    }
}