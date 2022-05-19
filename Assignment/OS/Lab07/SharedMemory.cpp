#include <cstdlib>
#include <cstring>
#include <iostream>
#include <random>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

int main() {
    pid_t p;
    p = fork();
    if (p == 0) {
        key_t key = ftok(".", 3);
        int   shmid = shmget(key, 1024, IPC_EXCL | 0666);
        if (shmid == -1) {
            cout << "shmget error" << endl;
            return -1;
        }
        void* addr = shmat(shmid, NULL, SHM_W);

        random_device rd;
        unsigned int  random = rd();
        memcpy(addr, &random, sizeof(random));
        cout << "write random number: " << random << endl;
        shmdt(addr);
    }
    else if (p > 0) {
        key_t key = ftok(".", 3);
        int   shmid = shmget(key, 1024, IPC_EXCL | 0666);
        if (shmid == -1) {
            cout << "shmget error" << endl;
            return -1;
        }
        void* addr = shmat(shmid, NULL, SHM_R);

        unsigned int random;
        waitpid(-1, NULL, 0);
        memcpy(&random, addr, sizeof(random));
        cout << "read random number: " << random << endl;
        shmdt(addr);
    }
    return 0;
}