#include "../h/kernel/BitMasks.h"
#include "../h/kernel/RegisterUtils.h"
#include "../h/kernel/TrapHandler.h"
#include "../h/kernel/Collector.h"
#include "../h/kernel/TCB.h"
#include "../h/syscall_c.h"
#include "../lib/console.h"
#include "../h/kernel/ConsoleUtils.h"

void enableInterrupts() {
    using namespace kernel::BitMasks;
    SYS_REGISTER_SET_BITS(sstatus, sstatus::SIE);
}

void disableInterrupts() {
    using namespace kernel::BitMasks;
    SYS_REGISTER_CLEAR_BITS(sstatus, sstatus::SIE);
}

void main() {
    // Set main trap handler
    using kernel::TrapHandler;
    WRITE_TO_SYS_REGISTER(&TrapHandler::supervisorTrap, stvec);

    //enableInterrupts();

    kernel::TCB::initializeMainThread();

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

    void userMain();
    userMain();

    //disableInterrupts();

    kernel::Collector::cleanup();
}

