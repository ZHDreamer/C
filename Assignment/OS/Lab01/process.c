#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid;
    pid = fork();
    if (pid > 0) {
        waitpid(-1, NULL, 0);
        printf("father\n");
    }
    else if (pid == 0) {
        printf("son\n");
        char  s[] = "hello.txt";
        FILE* fp;
        fp = fopen(s, "r");
        char buf[100] = {0};
        for (int i = 0; !feof(fp); i++) {
            fscanf(fp, "%c", &buf[i]);
        }
        printf("%s\n", buf);
    }
    else {
        printf("fork error");
    }
    printf("finish %d\n", pid);
    return 0;
}