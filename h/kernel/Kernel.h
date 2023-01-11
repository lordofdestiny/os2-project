//
// Created by os on 7/19/22.
//

#ifndef PROJECT_KERNEL_H
#define PROJECT_KERNEL_H
#include "./Memory/Cache.h"
#include "./Memory/slab.h"
#include "../../h/syscall_c.h"
#include "./Console/Console.h"

namespace kernel
{
    namespace memory
    {
        class Cache;
    }
    class Kernel final
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
        static constexpr size_t stackSize = DEFAULT_STACK_SIZE;
        static uint8* kernelStack;
        static uint8* kernelStackTopAddress asm("__kernelStack");
        static uint64 kernel_vt asm("__kernel_vt");
        static uint64 user_vt asm("__user_vt");

        static memory::Cache* cache_list;
    public:
        static bool isValidCache(void* cachep);
        static void insertIntoCacheList(void* cache);
        static void removeFromCacheList(void* cache);
    };
} // kernel

#endif //PROJECT_KERNEL_H
