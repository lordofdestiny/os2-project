//
// Created by os on 7/16/22.
//
#include "../../../h/kernel/SystemCalls/SystemCalls.h"
#include "../../../h/kernel/Memory/HeapAllocator.h"
#include "../../../h/kernel/Thread.h"
#include "../../../h/kernel/Scheduler.h"
#include "../../../h/syscall_c.h"
#include "../../../h/kernel/Semaphore.h"
#include "../../../h/kernel/Console/Console.h"

namespace kernel
{
    SystemCalls* SystemCalls::instance = nullptr;

    SystemCallHandler SystemCalls::getHandler(SystemCallCode type)
    {
        auto group = resolveGroup(type);
        if (group == nullptr) return nullptr;
        return group->getCall(type);
    }

    void* SystemCalls::operator new(size_t size)
    {
        return kmalloc(size);
    }
    void SystemCalls::operator delete(void* ptr)
    {
        return kfree(ptr);
    }

    SystemCalls& SystemCalls::getInstance()
    {
        return *instance;
    }
    CallHandlerGroup* SystemCalls::resolveGroup(SystemCallCode type)
    {
        for (auto g : systemCallGroups)
        {
            if (g->handles(type)) return g;
        }
        return nullptr;
    }

    void SystemCalls::registerCall(SystemCallCode type, SystemCallHandler hanlder)
    {
        auto group = resolveGroup(type);
        if (group == nullptr) return;

        group->registerCall(type, hanlder);
    }

    SystemCalls::~SystemCalls()
    {
        for (size_t i = 0; i < groupCount; i++)
        {
            delete systemCallGroups[i];
        }
    }


    void SystemCalls::initialize()
    {
        instance = new SystemCalls();
        instance->registerCall(SystemCallCode::MemoryAllocate, &mem_alloc);
        instance->registerCall(SystemCallCode::MemoryFree, &mem_free);

        instance->registerCall(SystemCallCode::ThreadCreate, &thread_create);
        instance->registerCall(SystemCallCode::ThreadExit, &thread_exit);
        instance->registerCall(SystemCallCode::ThreadDispatch, &thread_dispatch);
        instance->registerCall(SystemCallCode::ThreadInit, &thread_init);
        instance->registerCall(SystemCallCode::ThreadStart, &thread_start);
        instance->registerCall(SystemCallCode::ThreadDestroy, &thread_destroy);

        instance->registerCall(SystemCallCode::SemaphoreOpen, &sem_open);
        instance->registerCall(SystemCallCode::SemaphoreClose, &sem_close);
        instance->registerCall(SystemCallCode::SemaphoreWait, &sem_wait);
        instance->registerCall(SystemCallCode::SemaphoreSignal, &sem_signal);

        instance->registerCall(SystemCallCode::TimeSleep, &time_sleep);

        instance->registerCall(SystemCallCode::GetChar, &getc);
        instance->registerCall(SystemCallCode::PutChar, &putc);

        instance->registerCall(SystemCallCode::EnterUserMode, &enter_user_mode);
    }

    SystemCalls::SystemCalls() = default;

    void SystemCalls::mem_alloc()
    {
        auto blockCount = ACCEPT(size_t, 1);
        auto memory = ALLOCATOR.allocateBlocks(blockCount);
        RETURN(memory);
    }

    void SystemCalls::mem_free()
    {
        auto memory = ACCEPT(void*, 1);
        int code = ALLOCATOR.deallocateBlocks(memory);
        RETURN(code);
    }

    void SystemCalls::thread_create()
    {
        thread_init();
        auto handle = ACCEPT(thread_t*, 1);
        auto thread = (Thread*)*handle;
        RETURN_IF(thread == nullptr, -0x02);
        thread->setStatus(Thread::Status::READY);
        thread_start();
    }

    void SystemCalls::thread_exit()
    { // Handle if attempting to exit main
        auto runningThread = Thread::getRunning();
        RETURN_IF(runningThread == Thread::getMainThread(), -0x01);
        delete runningThread;
        Thread::dispatch();
        RETURN(0);
    }

    void SystemCalls::thread_dispatch()
    {
        Thread::dispatch();
    }

    void SystemCalls::thread_init()
    {
        auto handle = ACCEPT(thread_t*, 1);
        RETURN_IF(handle == nullptr, -0x01);
        auto task = ACCEPT(Thread::Task, 2);
        RETURN_IF(task == nullptr, -0x4);
        auto argument = ACCEPT(void*, 3);
        auto stack = ACCEPT(void*, 4);
        RETURN_IF(stack == nullptr, -0x3);
        auto mode = Thread::getRunning()->getMode();
        auto thread = new Thread(task, argument, stack, mode);
        if (thread == nullptr)
        {
            ALLOCATOR.deallocateBlocks(stack);
        }
        else
        {
            *handle = (thread_t)thread;
            thread->setHandle(handle); // to ensure not to delete thread tha was completed
        }
        RETURN_IF(thread == nullptr, -0x01);
        RETURN(0x00);
    }

    void SystemCalls::thread_start()
    {
        auto thread = (Thread*)*ACCEPT(thread_t*, 1);
        if (thread == nullptr)
        {
            RETURN(-0x01);
            return;
        }
        switch (thread->getStatus())
        {
        case Thread::Status::CREATED:
            thread->setStatus(Thread::Status::READY);
        case Thread::Status::READY:
            SCHEDULER.put(thread);
            RETURN(0);
        default:
            return;
        }
    }

    void SystemCalls::thread_destroy()
    {
        auto handle = ACCEPT(thread_t*, 1);
        auto thread = (Thread*)*handle;
        RETURN_IF(thread == nullptr, -0x01);
        if (thread->getStatus() == Thread::Status::CREATED)
        {
            delete thread;
        }
    }

    void SystemCalls::sem_open()
    {
        auto handle = ACCEPT(sem_t*, 1);
        auto init = ACCEPT(unsigned, 2);
        auto semaphore = new Semaphore((int)init);
        RETURN_IF(semaphore == nullptr, -0x01);
        *handle = (sem_t)semaphore;
        RETURN(0);
    }

    void SystemCalls::sem_close()
    {
        auto handle = ACCEPT(Semaphore*, 1);
        RETURN_IF(handle == nullptr, -0x01);
        delete handle;
        RETURN(0);
    }

    void SystemCalls::sem_wait()
    {
        auto id = ACCEPT(Semaphore*, 1);
        RETURN_IF(id == nullptr, -0x01);
        RETURN(0);
        id->wait();
    }

    void SystemCalls::sem_signal()
    {
        auto id = ACCEPT(Semaphore*, 1);
        RETURN_IF(id == nullptr, -0x01);
        RETURN(0);
        id->signal();
    }

    void SystemCalls::time_sleep()
    {
        auto thread = Thread::getRunning();
        auto ticks = ACCEPT(time_t, 1);
        RETURN_IF(ticks < 0, -0x01);
        RETURN(0);
        SCHEDULER.entrance(thread, ticks);
    }

    void SystemCalls::getc()
    {
        auto c = CONSOLE.readChar();
        if (c > 127) c = EOF;
        RETURN(c);
    }

    void SystemCalls::putc()
    {
        auto c = ACCEPT(char, 1);
        CONSOLE.writeChar(c);
    }

    void SystemCalls::enter_user_mode()
    {
        auto thread = Thread::getRunning();
        thread->enterUserMode();
    }

}