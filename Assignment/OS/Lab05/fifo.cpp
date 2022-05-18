#include <fcntl.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

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
            cout << "The data of buffer: " << buf << endl;
            read(fd, buf, sizeof(buf));
            cout << "The message from the pipe is: " << buf << endl;
            close(fd);
            cout << "father close the fifo successfully"<< endl;
        }
        else if (pid == 0) {
            char buf[] = "fifo test";
            fd = open(FIFO_SERVER, O_WRONLY);
            cout << "child write success!" << endl;
            write(fd, buf, sizeof(buf));
            close(fd);
            cout << "child close the fifo successfully" << endl;
        }
        else {
            cout << "fork error" << endl;
        }
        return 0;
    }
}
