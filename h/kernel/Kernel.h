//
// Created by os on 7/19/22.
//

#ifndef PROJECT_KERNEL_H
#define PROJECT_KERNEL_H
#include "./MemoryAllocator.h"
#include "./Scheduler.h"
#include "./TrapHandlers.h"
#include "./BitMasks.h"

namespace kernel {
    class Kernel {
    public:
        static void initialize();
        static void finalize();
    private:
        static void enableInterrupts();
        static void disableInterrupts();
        static void setTrapHandler();
        static void waitForUserThreads();
    public:
        static const size_t stackSize = DEFAULT_STACK_SIZE * sizeof(uint64);
        alignas(uint16) static uint8 kernelStack[stackSize];
        static void* kernelStackTopAddress asm("__kernelStack");
    };
} // kernel

#endif //PROJECT_KERNEL_H
