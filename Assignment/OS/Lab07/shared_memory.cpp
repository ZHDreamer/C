#include <cstring>
#include <random>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/wait.h>

using namespace std;

int main() {
    pid_t p;
    p = fork();
    key_t key = ftok(".", 3);
    int   shmid = shmget(key, 1024, IPC_CREAT | 0666);
    if (shmid == -1) {
        printf("shmget error\n");
        return -1;
    }
    void* addr = shmat(shmid, NULL, SHM_R);
    if (p == 0) {
        random_device rd;
        unsigned int  random = rd();
        memcpy(addr, &random, sizeof(random));
        printf("write random number: %d\n", random);
        shmdt(addr);
    }
    else if (p > 0) {
        unsigned int random;
        waitpid(-1, NULL, 0);
        memcpy(&random, addr, sizeof(random));
        printf("read random number: %d\n", random);
        shmdt(addr);
    }
    return 0;
}