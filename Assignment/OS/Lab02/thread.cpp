#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int val = 0;

void* create(void* arg) {
    printf("shared val in sub thread: %d\n", val);
    val += 1;
    printf("shared val +1 in sub thread\n");
    printf("shared val in sub thread: %d\n", val);
    return (void*)0;
}

int main() {
    printf("shared val in main thread: %d\n", val);
    int       error;
    pthread_t tid;
    error = pthread_create(&tid, NULL, create, NULL);
    if (error) {
        printf("create pthread error\n");
        return -1;
    }
    void* p;
    error = pthread_join(tid, &p);
    if (error) {
        printf("join pthread error\n");
        return -1;
    }
    printf("shared val in main thread: %d\n", val);
    return 0;
}