//
// Created by os on 7/19/22.
//

#ifndef PROJECT_KERNEL_H
#define PROJECT_KERNEL_H
#include "./Memory/HeapAllocator.h"
#include "./Scheduler.h"
#include "./TrapHandlers.h"
#include "./Utils/BitMasks.h"
#include "../../h/syscall_c.h"

namespace kernel
{
    class Kernel
    {
    public:
        using TMain = void (*)(void);
        static void initialize();
        static void execute(TMain userMain);
        static void finalize();
    private:
        static void enableInterrupts();
        static void setTrapHandler(bool blockOnError = false);
        static void waitForUserThreads();
    private:
        static const size_t stackSize = DEFAULT_STACK_SIZE * sizeof(uint64);
        alignas(uint16) static uint8 kernelStack[stackSize];
        static uint8* kernelStackTopAddress asm("__kernelStack");

        static sem_t userMainDone;
        static thread_t userMainThread;
    };
} // kernel

#endif //PROJECT_KERNEL_H
