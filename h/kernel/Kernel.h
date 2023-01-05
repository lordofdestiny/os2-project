//
// Created by os on 7/19/22.
//

#ifndef PROJECT_KERNEL_H
#define PROJECT_KERNEL_H
#include "./Memory/Cache.h"
#include "./Memory/slab.h"
#include "../../h/syscall_c.h"

namespace kernel
{
    namespace memory
    {
        class Cache;
    }
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
        static constexpr size_t stackSize = DEFAULT_STACK_SIZE;
        static uint8* kernelStack;
        static uint8* kernelStackTopAddress asm("__kernelStack");

        static sem_t userMainDone;
        static thread_t userMainThread;
        static kernel::memory::Cache* cache_list;
        static constexpr int free_pages_low = 20;
        static constexpr int free_pages_high = 40;
    public:
        static bool isValidCache(void* cachep);
        static void insertIntoCacheList(void* cache);
        static void removeFromCacheList(void* cache);

    };
} // kernel

#endif //PROJECT_KERNEL_H
