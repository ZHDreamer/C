#include <cstdlib>
#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>

using namespace std;

typedef struct {
    char name[5];
    int  age;
} People;

int main() {
    pid_t p;
    p = fork();
    if (p == 0) {
        key_t key = ftok(".", 4);
        int   shmid = shmget(key, 1024, IPC_CREAT | 0666);

        if (shmid == -1) {
            cout << "shmget error" << endl;
            return -1;
        }
        void*  addr = shmat(shmid, NULL, SHM_W);
        sleep(10);
        People p = {"zmh", 20};
        memcpy(addr, &p, sizeof(People));
        shmdt(addr);
    }
    else if (p > 0) {
        key_t key = ftok(".", 4);
        int shmid = shmget(key, 1024, IPC_EXCL);
        void * addr = shmat(shmid, NULL, SHM_W);

        People * p = (People*)malloc(sizeof(People));
        // waitpid(-1, NULL, 0);
        memcpy(p, addr, sizeof(People));
        cout << "name = " << p->name << endl;
        cout << "age = " << p->age << endl;
        shmdt(addr);
    }
    return 0;
}