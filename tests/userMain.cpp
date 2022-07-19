#include "../h/syscall_c.h"
#include "../h/kernel/ConsoleUtils.h"

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

//    int i = 0;
//    while(i < 3) {
//        i++;
//        thread_dispatch();
//    }
}