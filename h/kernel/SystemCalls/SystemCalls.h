//
// Created by os on 7/16/22.
//

#ifndef PROJECT_SYSTEMCALLS_H
#define PROJECT_SYSTEMCALLS_H
#include "./SystemCallCode.h"
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

    enum class CallType
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

    class SystemCalls
    {
    public:
        SystemCallHandler getHandler(CallType type);
        static SystemCalls& getInstance();
        SystemCallHandlerGroup* resolveGroup(CallType type);
        void initialize();
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
        void registerSystemCall(CallType type, SystemCallHandler hanlder);
        static const int groupCount = 6;
        SystemCallHandlerGroup* memoryCalls = new SystemCallHandlerGroup(SystemCallType::Memory);
        SystemCallHandlerGroup* threadCalls = new SystemCallHandlerGroup(SystemCallType::Thread);
        SystemCallHandlerGroup* semaphoreCalls = new SystemCallHandlerGroup(SystemCallType::Semaphore);
        SystemCallHandlerGroup* timerCalls = new SystemCallHandlerGroup(SystemCallType::Timer);
        SystemCallHandlerGroup* consoleCalls = new SystemCallHandlerGroup(SystemCallType::Console);
        SystemCallHandlerGroup* systemCalls = new SystemCallHandlerGroup(SystemCallType::System);
        SystemCallHandlerGroup* systemCallGroups[groupCount];
    };
} // kernel

#endif //PROJECT_SYSTEMCALLS_H
