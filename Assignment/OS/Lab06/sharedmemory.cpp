#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

#define MAXSEM 5
#define NUM 10

// struct sembuf {
//     unsigned short sem_num;  // semaphore number
//     short          sem_op;   // semaphore operation
//     short          sem_flg;  // operation flags
// };

union semun {
    int              val;
    struct semid_ds* buf;
    unsigned short*  arry;
};

int sem_init(key_t key, int value) {
    int sem_id = semget(key, 1, 0666 | IPC_CREAT);
    if (semctl(sem_id, 0, SETVAL, value) == -1) {
        cout << "sem init error " << key << endl;
    }
    return sem_id;
}

int get_sem(int sem_id) {
    int val = semctl(sem_id, 0, GETVAL);
    return val;
}

void del_sem(int sem_id) {
    semctl(sem_id, 0, IPC_RMID);
}

void P(int sem_id) {
    struct sembuf sem_p;

    sem_p.sem_flg = SEM_UNDO;
    sem_p.sem_num = 0;
    sem_p.sem_op = -1;

    if (semop(sem_id, &sem_p, 1) == -1) {
        cout << "P error" << endl;
    }
}

void V(int sem_id) {
    struct sembuf* sem_v = (struct sembuf*)malloc(sizeof(struct sembuf));

    sem_v->sem_flg = SEM_UNDO;
    sem_v->sem_num = 0;
    sem_v->sem_op = 1;

    if (semop(sem_id, sem_v, 1) == -1) {
        cout << "V error" << endl;
    }
}

int main() {
    int fullid, emptyid, mutxid;
    // init semaphore
    fullid = sem_init(2, 0);
    emptyid = sem_init(3, MAXSEM);
    mutxid = sem_init(4, 1);

    pid_t producer = fork();
    if (producer == 0) {
        for (int i = 0; i < NUM; i++) {
            P(emptyid);
            P(mutxid);
            cout << "producer  produced number " << get_sem(fullid) + 1 << endl;
            V(fullid);
            V(mutxid);
            sleep(1);
        }
        sleep(5);
    }
    else {
        pid_t consumer1 = fork();
        if (consumer1 == 0) {
            for (int i = 0; i < NUM / 2; i++) {
                P(fullid);
                P(mutxid);
                cout << "consumer A  current number " << get_sem(fullid)
                     << endl;
                V(emptyid);
                V(mutxid);
                sleep(2);
            }
            sleep(3);
        }
        else if (consumer1 > 0) {
            pid_t consumer2 = fork();
            if (consumer2 == 0) {
                for (int i = 0; i < NUM / 2; i++) {
                    P(fullid);
                    P(mutxid);
                    cout << "consumer B  current number " << get_sem(fullid)
                         << endl;
                    V(emptyid);
                    V(mutxid);
                    sleep(3);
                }
            }
            else if (consumer2 > 0) {
                while (wait(0) != -1);

                del_sem(fullid);
                del_sem(emptyid);
                del_sem(mutxid);
            }
        }
    }
    return 0;
}