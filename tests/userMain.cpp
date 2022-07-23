#include "../h/kernel/ConsoleUtils.h"
#include "../h/syscall_c.h"

struct SharedData{
    int arr[2];
    int cycles = 1;
    sem_t sem;
    bool done[3];
    SharedData() {
        sem_open(&sem,1);
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
            time_sleep(5);
            printString("Hello from A!\n");
            thread_dispatch();
        }
        thread_exit();
    }, nullptr);

    SharedData data;

    thread_t thread2;
    thread_create(&thread2, [](void * ptr) {
        time_sleep(10);
        auto data = (SharedData*)ptr;
        while(!data->done[0] && !data->done[1] && !data->done[2]) {
            time_sleep(5);
            printString("Thread B says hi!!!\n");
        };
    }, &data);

    thread_t thread3;
    thread_create(&thread3, [](void *) {
        for(int i = 0; i < 3;i++){
            if (i == 2) time_sleep(20);
            else time_sleep(3);
            printString("Hello from C!\n");
            thread_dispatch();
        }
    }, nullptr);

    thread_t threadA;
    thread_create(&threadA,[](void* arg){
        auto data = (SharedData*) arg;
        for(int i = 0; i < data->cycles; i++) {
            sem_wait(data->sem);
            data->arr[0] = i;
            sem_signal(data->sem);
            thread_dispatch();
        }
        data->done[0] = true;
    }, &data);

    thread_t threadB;
    thread_create(&threadB,[](void* arg){
        auto data = (SharedData*) arg;
        for(int i = 3; i < 3 + data->cycles; i++){
            sem_wait(data->sem);
            data->arr[1] = i;
            sem_signal(data->sem);
            thread_dispatch();
        }
        data->done[1] = true;
    }, &data);

    thread_t threadC;
    thread_create(&threadC,[](void* arg){
        auto data = (SharedData*) arg;
        for(int i = 0; i < data->cycles; i++){
            sem_wait(data->sem);
            char str[5];
            str[0] = data->arr[0]+48;
            str[1] = ' ';
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
    printString("???\n");
    char buffer[40];
    getString(buffer,30);
    int num = stringToInt(buffer);
    num += 25;
    printString("Calculating...\n");
    time_sleep(25);
    printString("Result: ");
    printInt(num,8);
    putc('\n');
    time_sleep(5);

    thread_t phantom;
    thread_create(&phantom, [](void*arg){
        printString("Phantom starting!");
//        time_sleep(15);
        printString("Phantom done!");
    }, nullptr);

}