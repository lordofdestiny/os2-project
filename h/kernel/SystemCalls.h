//
// Created by os on 7/16/22.
//

#ifndef PROJECT_SYSTEMCALLS_H
#define PROJECT_SYSTEMCALLS_H

namespace kernel {
    class SystemCalls {
    public:
        SystemCalls()=delete;
        SystemCalls(SystemCalls const&)=delete;
        SystemCalls& operator=(SystemCalls const&)=delete;
        enum class Type {
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

        static void handle();
        static void environmentCall(Type type) asm("__environmentCall__");
    private:
        static void mem_alloc();
        static void mem_free();
        static void thread_create();
        static void thread_exit();
        static void sem_open();
        static void sem_close();
        static void sem_wait();
        static void sem_signal();
        static void time_sleep();
        static void getc();
        static void putc();
    };
} // kernel

#endif //PROJECT_SYSTEMCALLS_H
