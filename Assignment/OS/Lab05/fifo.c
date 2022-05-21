#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define FIFO_SERVER "fifoserver"

int main() {
    int error;
    unlink(FIFO_SERVER);
    error = mkfifo(FIFO_SERVER, 0777);
    if (error == 0) {
        pid_t pid;
        pid = fork();
        int fd;
        if (pid > 0) {
            fd = open(FIFO_SERVER, O_RDONLY);
            char buf[80];
            read(fd, buf, sizeof(buf));
            printf("The message from the pipe is: %s\n", buf);
            close(fd);
            printf("father close the fifo successfully\n");
        }
        else if (pid == 0) {
            char buf[] = "fifo test";
            fd = open(FIFO_SERVER, O_WRONLY);
            printf("child write success!\n");
            write(fd, buf, sizeof(buf));
            close(fd);
            printf("child close the fifo successfully\n");
        }
        else {
            printf("fork error\n");
        }
        return 0;
    }
}
