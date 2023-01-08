//
// Created by os on 7/16/22.
//

#include "../../h/kernel/Thread.h"
#include "../../h/kernel/Scheduler.h"
#include "../../h/kernel/Memory/MemoryManager.h"
#include "../../h/kernel/Memory/HeapAllocator.h"
#include "../../h/kernel/Utils/BitMasks.h"
#include "../../h/kernel/Utils/Utils.h"
#include "../../h/kernel/Memory/slab.h"
#include "../../h/syscall_c.h"

namespace kernel
{
    kmem_cache_t* Thread::object_cache = nullptr;
    Thread* Thread::mainThread = nullptr;
    Thread* Thread::runningThread = nullptr;
    time_t Thread::runningTimeLeft = DEFAULT_TIME_SLICE;
    bool Thread::mainFinished = false;
    uint64 Thread::threadIdSource = 0;
    uint64 Thread::threadCounter[2] = { 0, 0 };

    Thread::Context::Context(uint64 pc, uint64 status):
        pc(pc), sstatus(status), registers()
    { }

    Thread::Context::Registers::Registers()
    {
        utils::memset(this, 0, sizeof(*this));
    }

    void* Thread::operator new(size_t size) noexcept
    {
        return kmem_cache_alloc(object_cache);
    }

    void Thread::operator delete(void* ptr) noexcept
    {
        kmem_cache_free(object_cache, ptr);
    }

    void Thread::initialize()
    {
        object_cache = kmem_cache_create(
            "thread_t", sizeof(Thread),
            nullptr, nullptr);

        runningThread = getMainThread();
        runningTimeLeft = DEFAULT_TIME_SLICE;
    }

    void Thread::dispatch()
    {

        auto oldThread = runningThread;

        if (oldThread &&
            oldThread->status != Status::BLOCKED)
        {
            SCHEDULER.put(oldThread);
        }

        auto newThread = SCHEDULER.get();

        runningThread = newThread;
        runningThread->status = Status::RUNNING;
        runningTimeLeft = DEFAULT_TIME_SLICE;
    }

    Thread* Thread::getMainThread()
    {
        if (mainThread == nullptr)
        {
            mainThread = new Thread(
                nullptr, nullptr,
                nullptr, Mode::SYSTEM);
        }
        return mainThread;
    }

    Thread* Thread::getRunning()
    {
        return runningThread;
    }

    void Thread::shelveRunning()
    {
        runningThread = nullptr;
    }

    uint64 Thread::getCount(Thread::Mode mode)
    {
        return threadCounter[(int)mode];
    }


    void Thread::taskWrapper()
    {
        runningThread->task(runningThread->arg);
        thread_exit();
    }

    uint64 Thread::sstatusGetInitial(Mode mode)
    {
        using namespace BitMasks;
        using BitMasks::sstatus;
        const auto status =
            runningThread == nullptr
            ? (uint64)sstatus::SPIE
            : runningThread->context.sstatus;
        const auto SPP =
            mode == Mode::SYSTEM
            ? (uint64)sstatus::SPP
            : 0x00;
        const auto SPIE = status & (uint64)sstatus::SPIE;
        return SPP | SPIE | (uint64)sstatus::SIE;
    }

    uint64 Thread::pcGetInitial(Task function)
    {
        if (function == nullptr) return 0x00;
        return (uint64)taskWrapper;
    }

    void Thread::setMainFinished()
    {
        mainFinished = true;
    }

    bool Thread::isMainFinished()
    {
        return mainFinished;
    }

    Thread::Thread(Task function, void* argument, void* stack, Mode mode):
        context(pcGetInitial(function), sstatusGetInitial(mode)),
        task(function), arg(argument), stack((uint64*)stack), mode(mode)
    {
        if (stack != nullptr)
        {
            auto stackTop = (uint64)((char*)this->stack + DEFAULT_STACK_SIZE);
            context.registers.sp = stackTop;
        }
        threadCounter[(int)mode]++;
    }

    Thread::~Thread()
    {
        threadCounter[(int)mode]--;
        if (this == runningThread)
        {
            runningThread = nullptr;
        }
        if (stack != nullptr)
        {
            using namespace memory;
            if (UserHeap().start <= stack
                && stack < UserHeap().end)
            {
                ALLOCATOR.deallocateBlocks(stack);
            }
            else if (KernelHeap().start <= stack
                && stack < KernelHeap().end)
            {
                kfree(stack);
            }
        }
        *my_handle = nullptr;
    }

    void Thread::tick()
    {
        if (this == runningThread
            && --runningTimeLeft == 0)
        {
            dispatch();
        }
        else if (sleepingTime > 0)
        {
            sleepingTime--;
        }
    }

    void Thread::enterUserMode()
    {
        if (this->mode == Mode::SYSTEM)
        {
            using namespace BitMasks;
            auto newsstatus =
                context.sstatus
                & (~(uint64)sstatus::SPP);

            context.sstatus = newsstatus;

            mode = Mode::USER;

            threadCounter[(int)Mode::SYSTEM]--;
            threadCounter[(int)Mode::USER]++;
        }
    }

} // kernel