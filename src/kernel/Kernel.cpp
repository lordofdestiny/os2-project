//
// Created by os on 7/19/22.
//
#include "../../h/kernel/Kernel.h"
#include "../../h/kernel/TrapHandlers.h"
#include "../../h/kernel/Utils/BitMasks.h"
#include "../../h/kernel/Memory/slab.h"
#include "../../h/kernel/Memory/MemoryManager.h"
#include "../../h/kernel/Memory/HeapAllocator.h"
#include "../../h/kernel/Utils/RegisterUtils.h"
#include "../../h/kernel/SystemCalls/SystemCalls.h"
#include "../../h/kernel/Console/Console.h"
#include "../../h/kernel/Memory/BuddyAllocator.h"
#include "../../h/syscall_cpp.hpp"

#define BLOCK_ON_ERROR

#ifdef BLOCK_ON_ERROR
const static bool block = true;
#else
const static bool block = false;
#endif


namespace kernel
{
    uint8* Kernel::kernelStack = nullptr;
    uint8* Kernel::kernelStackTopAddress = nullptr;
    memory::Cache* Kernel::cache_list = nullptr;

    void Kernel::initialize()
    {
        using namespace memory;
        const auto mem_range = KernelHeap();
        kmem_init(mem_range.start, mem_range.size() / BLOCK_SIZE);
        heap::initialize(UserHeap());
        /* allocate kernel stack */
        kernelStack = (uint8*)kmalloc(stackSize);
        kernelStackTopAddress = (uint8*)kernelStack + stackSize;

        setTrapHandler(block);
        SystemCalls::initialize();
        Scheduler::initialize();
        Semaphore::initialize();
        Thread::initialize();
        Console::initialize();
        enableInterrupts();
    }

    void Kernel::execute(TMain userMain)
    {
        struct Runner: ::Thread
        {
            TMain main;
            sem_t sem;
            Runner(TMain main)
                :Thread(), main(main)
            {
                sem_open(&sem, 0);
                start();
            }
            void run() override
            {
                enter_user_mode();
                main();
                sem_signal(sem);
            }
            ~Runner() override
            {
                sem_wait(sem);
                sem_close(sem);
            }
        } main{ userMain };
    }

    void Kernel::finalize()
    {
        waitForUserThreads();
        Thread::setMainFinished();
        CONSOLE.join();
        delete Console::instance;
        // delete
    }

    void Kernel::enableInterrupts()
    {
        // Enable external interrupts
        SREGISTER_SET_BITS(sstatus, BitMasks::sstatus::SIE);
    }

    void Kernel::setTrapHandler(bool blockOnError)
    {
        TrapHandlers::Handler errorHandler;
        if (blockOnError)
        {
            SREGISTER_READ(stvec, errorHandler);
            TrapHandlers::setErrorHandler(errorHandler);
        }
        SREGISTER_WRITE(stvec, &TrapHandlers::supervisorTrap);
    }

    void Kernel::waitForUserThreads()
    {
        while (Thread::getCount(Thread::Mode::USER) > 0)
        {
            thread_dispatch();
        }
    }

    bool Kernel::isValidCache(void* cachep)
    {
        if (cachep == nullptr) return false;
        auto cache = (memory::Cache*)cache_list;
        do
        {
            if (cache == cachep) return true;
            cache = cache->next;
        } while (cache != cache_list);
        return false;
    }

    void Kernel::insertIntoCacheList(void* cachep)
    {
        if (cachep == nullptr) return;
        auto cache = (memory::Cache*)cachep;
        if (cache_list == nullptr)
        {
            cache->prev = cache;
            cache->next = cache;
            cache_list = cache;
            return;
        }

        cache->next = cache_list;
        cache->prev = cache_list->prev;
        cache_list->prev->next = cache;
        cache_list->prev = cache;
    }

    void Kernel::removeFromCacheList(void* cachep)
    {
        if (cachep == nullptr ||
            cache_list == nullptr) return;
        auto cache = (memory::Cache*)cachep;

        const bool one = cache == cache->next;
        if (not one)
        {
            cache->prev->next = cache->next;
            cache->next->prev = cache->prev;
        }
        cache->prev = nullptr;
        cache->next = nullptr;
        if (one) cache_list = nullptr;
    }
} // kernel