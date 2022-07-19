//
// Created by os on 7/16/22.
//

#ifndef PROJECT_SYSTEMCALLS_H
#define PROJECT_SYSTEMCALLS_H

namespace kernel {
    namespace SystemCalls {
        enum class CallType {
            MemoryAllocate = 0x01,
            MemoryFree = 0x02,
            ThreadCreate = 0x11,
            ThreadExit = 0x12,
            ThreadDispatch = 0x13,
            SemaphoreOpen = 0x21,
            SemaphoreClose = 0x22,
            SemaphoreWait = 0x23,
            SemaphoreSignal = 0x24,
            TimeSleep = 0x31,
            GetChar = 0x41,
            PutChar = 0x42
        };
        void handle();
        void mem_alloc();
        void mem_free();
        void thread_create();
        void thread_exit();
        void sem_open();
        void sem_close();
        void sem_wait();
        void sem_signal();
        void time_sleep();
        void getc();
        void putc();
        void environmentCall(CallType type) asm("__environmentCall__");
    };
} // kernel

#endif //PROJECT_SYSTEMCALLS_H
