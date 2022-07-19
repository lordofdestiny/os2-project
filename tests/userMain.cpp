#include "../h/syscall_c.h"
#include "../h/kernel/ConsoleUtils.h"

struct SharedData{
    int arr[2];
    sem_t sem;
    bool done[3];
    SharedData() {
        sem_open(&sem,0);
        arr[0] = arr[1] = 0;
        for(int i = 0; i < 3; i++){
            done[i]=false;
        }
    }
    ~SharedData() {
        sem_close(sem);
    }
};

void userMain() {
    thread_t thread1;
    thread_create(&thread1, [](void *) {
        for(int i = 0 ; i < 2; i++){
            printString("Hello from A!\n");
            thread_dispatch();
        }
        thread_exit();
    }, nullptr);

    thread_t thread2;
    thread_create(&thread2, [](void *) {
        thread_exit();
        printString("Hello from B!\n");
        thread_dispatch();
    }, nullptr);

    thread_t thread3;
    thread_create(&thread3, [](void *) {
        for(int i = 0; i < 3;i++){
            if (i == 2) thread_exit();
            printString("Hello from C!\n");
            thread_dispatch();
        }
    }, nullptr);

    SharedData data;

    thread_t threadA;
    thread_create(&threadA,[](void* arg){
        auto data = (SharedData*) arg;
        for(int i = 0; i < 3; i++) {
            sem_wait(data->sem);
            data->arr[0] = i;
            thread_dispatch();
            sem_signal(data->sem);
            thread_dispatch();
        }
        data->done[0] = true;
    }, &data);

    thread_t threadB;
    thread_create(&threadB,[](void* arg){
        auto data = (SharedData*) arg;
        for(int i = 3; i < 6; i++){
            sem_wait(data->sem);
            data->arr[1] = i;
            thread_dispatch();
            sem_signal(data->sem);
            thread_dispatch();
        }
        data->done[1] = true;
    }, &data);

    thread_t threadC;
    thread_create(&threadC,[](void* arg){
        auto data = (SharedData*) arg;
        for(int i = 0; i < 3; i++){
            sem_wait(data->sem);
            char str[5];
            str[0] = data->arr[0]+48;
            str[1] = ' ';
            thread_dispatch();
            str[2] = data->arr[1]+48;
            str[3] = '\n';
            str[4] = '\0';
            printString(str);
            sem_signal(data->sem);
            thread_dispatch();
        }
        data->done[2] = true;
    }, &data);

    while(!data.done[0] && !data.done[1] && !data.done[2]){
        thread_dispatch();
    }
}