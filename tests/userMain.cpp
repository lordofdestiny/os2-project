#include "../h/syscall_c.h"
#include "../h/kernel/ConsoleUtils.h"

void userMain() {
    thread_t thread1;
    thread_create(&thread1,[](void*){
        while(true) {
            printString("Hello from A!\n");
            thread_dispatch();
        }
    }, nullptr);


    thread_t thread2;
    thread_create(&thread2,[](void*){
        while(true) {
            printString("Hello from B!\n");
            thread_dispatch();
        }
    }, nullptr);

    while(true) thread_dispatch();
}