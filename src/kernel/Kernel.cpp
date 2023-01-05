//
// Created by os on 7/19/22.
//
#include "../../h/kernel/Kernel.h"
#include "../../h/kernel/TrapHandlers.h"
#include "../../h/kernel/Utils/BitMasks.h"
#include "../../h/kernel/Memory/slab.h"
#include "../../h/kernel/Memory/MemoryManager.h"
#include "../../h/kernel/Utils/RegisterUtils.h"
#include "../../h/kernel/SystemCalls/SystemCalls.h"
#include "../../h/kernel/Console/Console.h"
#include "../../h/kernel/Memory/BuddyAllocator.h"

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
    sem_t Kernel::userMainDone = nullptr;
    thread_t Kernel::userMainThread = nullptr;
    memory::Cache* Kernel::cache_list = nullptr;

    void Kernel::initialize()
    {
        auto kmem = memory::kernelSectionBounds();
        kmem_init(kmem.startAddress, kmem.size() / BLOCK_SIZE);

        /* allocate kernel stack */
        kernelStack = (uint8*)kmalloc(Kernel::stackSize);
        kernelStackTopAddress = (uint8*)kernelStack + stackSize;

        setTrapHandler(block);
        SYSTEMCALLS.initialize();
        Semaphore::initialize();
        Thread::initialize();
        CONSOLE.initialize();
        enableInterrupts();
        sem_open(&Kernel::userMainDone, 0);
    }

    void Kernel::execute(TMain userMain)
    {
        struct Shadow
        {
            TMain main;
            sem_t sem;
        } argument{ userMain, userMainDone };

        thread_create(&userMainThread,
            [](void* args)
            {
                {
                    auto sh = (Shadow*)args;
                    // enter_user_mode();
                    sh->main();
                    sem_signal(sh->sem);
                }
            }, &argument);
        sem_wait(userMainDone);
    }

    void Kernel::finalize()
    {
        sem_close(userMainDone);
        waitForUserThreads();
        Thread::setMainFinished();
        CONSOLE.join();
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
        while (Thread::getCount(Thread::Mode::USER) > 1)
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
        if (cachep == nullptr || cache_list == nullptr) return;
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