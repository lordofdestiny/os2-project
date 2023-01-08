//
// Created by os on 7/16/22.
//

#ifndef PROJECT_SYSTEMCALLS_H
#define PROJECT_SYSTEMCALLS_H
#include "./SystemCallGroup.h"

#define ACCEPT(type, index) (type) RUNNING_REGISTERS.a##index
#define RETURN(value) RUNNING_REGISTERS.a0 = (uint64) value


#define RETURN_IF(test, value)      \
    do{                             \
        if((test)) {                \
            RETURN(value);          \
            return;                 \
        }                           \
    }while(0)                       \


#define SYSTEMCALLS SystemCalls::getInstance()

namespace kernel
{
    enum class SystemCallType
    {
        Memory = 0x00,
        Thread = 0x10,
        Semaphore = 0x20,
        Timer = 0x30,
        Console = 0x40,
        System = 0xF0,
    };

    enum class SystemCallCode
    {
        MemoryAllocate = 0x01,
        MemoryFree = 0x02,
        ThreadCreate = 0x11,
        ThreadExit = 0x12,
        ThreadDispatch = 0x13,
        ThreadInit = 0x14,
        ThreadStart = 0x15,
        ThreadDestroy = 0x16,
        SemaphoreOpen = 0x21,
        SemaphoreClose = 0x22,
        SemaphoreWait = 0x23,
        SemaphoreSignal = 0x24,
        TimeSleep = 0x31,
        GetChar = 0x41,
        PutChar = 0x42,
        EnterUserMode = 0xFF
    };

    class SystemCalls final
    {
        static void* operator new(size_t size);
        static void operator delete(void* ptr);

    public:
        friend class Kernel;
        SystemCalls(SystemCalls const&) = delete;
        SystemCalls& operator=(SystemCalls const&) = delete;
        ~SystemCalls();
        static void initialize();
        static SystemCalls& getInstance();

        SystemCallHandler getHandler(SystemCallCode type);
        CallHandlerGroup* resolveGroup(SystemCallCode type);
    public:
        static void mem_alloc();
        static void mem_free();
        static void thread_create();
        static void thread_exit();
        static void thread_dispatch();
        static void thread_init();
        static void thread_start();
        static void thread_destroy();
        static void sem_open();
        static void sem_close();
        static void sem_wait();
        static void sem_signal();
        static void time_sleep();
        static void getc();
        static void putc();
        static void enter_user_mode();
    private:
        SystemCalls();
        void registerCall(SystemCallCode type, SystemCallHandler hanlder);
        static constexpr int groupCount = 6;
        static SystemCalls* instance;
        CallHandlerGroup* systemCallGroups[groupCount]{
            new CallHandlerGroup(SystemCallType::Memory),
            new CallHandlerGroup(SystemCallType::Thread),
            new CallHandlerGroup(SystemCallType::Semaphore),
            new CallHandlerGroup(SystemCallType::Timer),
            new CallHandlerGroup(SystemCallType::Console),
            new CallHandlerGroup(SystemCallType::System)
        };

    };
} // kernel

#endif //PROJECT_SYSTEMCALLS_H
