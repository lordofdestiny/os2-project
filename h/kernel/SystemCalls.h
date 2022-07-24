//
// Created by os on 7/16/22.
//

#ifndef PROJECT_SYSTEMCALLS_H
#define PROJECT_SYSTEMCALLS_H

#define ACCEPT(type, index) (type) RUNNING_REGISTERS.a##index
#define RETURN(value) RUNNING_REGISTERS.a0 = (uint64) value

#define RETURN_IF(test, value)      \
    do{                             \
        if((test)) {                \
            RETURN(value);          \
            return;                 \
        }                           \
    }while(0)                       \


namespace kernel {
    namespace SystemCalls {
        enum class CallType {
            MemoryAllocate = 0x01,
            MemoryFree = 0x02,
            ThreadCreate = 0x11,
            ThreadExit = 0x12,
            ThreadDispatch = 0x13,
            ThreadInit = 0x14,
            ThreadStart = 0x15,
            SemaphoreOpen = 0x21,
            SemaphoreClose = 0x22,
            SemaphoreWait = 0x23,
            SemaphoreSignal = 0x24,
            TimeSleep = 0x31,
            GetChar = 0x41,
            PutChar = 0x42,
            EnterUserMode = 0xFF
        };
        void mem_alloc();
        void mem_free();
        void thread_create();
        void thread_exit();
        void thread_init();
        void thread_start();
        void sem_open();
        void sem_close();
        void sem_wait();
        void sem_signal();
        void time_sleep();
        void getc();
        void putc();
        void enter_user_mode();
        void environmentCall(CallType type) asm("__environmentCall__");
    };
} // kernel

#endif //PROJECT_SYSTEMCALLS_H
