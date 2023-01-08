//
// Created by os on 7/16/22.
//

#include "../../h/kernel/Scheduler.h"
#include "../../h/kernel/Utils/Utils.h"

namespace kernel
{
    Scheduler* Scheduler::instance = nullptr;

    void* Scheduler::operator new(size_t size)
    {
        return kmalloc(size);
    }
    void Scheduler::operator delete(void* ptr)
    {
        return kfree(ptr);
    }

    void Scheduler::initialize()
    {
        instance = new Scheduler();
    }
    Scheduler& Scheduler::getInstance()
    {
        return *instance;
    }

    void Scheduler::put(kernel::Thread* thread)
    {
        if (thread == idleThread) return;

        thread->setStatus(Thread::Status::READY);
        if (readyHead == nullptr)
        {
            readyHead = thread;
        }
        else
        {
            readyTail->next = thread;
        }
        readyTail = thread;
    }

    Thread* Scheduler::get()
    {
        if (readyHead == nullptr) return getIdleThread();

        auto thread = utils::exchange(
            readyHead,
            readyHead->next);

        if (readyHead == nullptr)
        {
            readyTail = nullptr;
        }
        thread->next = nullptr;
        return thread;
    }

    void Scheduler::entrance(Thread* thread, uint64 ticks)
    {
        if (ticks == 0) return;

        thread->setStatus(Thread::Status::BLOCKED);

        Thread* prev = nullptr;
        Thread* curr = sleepingHead;
        while (curr != nullptr)
        {
            if (ticks <= curr->getSleepingTime()) break;
            ticks -= curr->getSleepingTime();
            prev = utils::exchange(curr, curr->next);
        }

        if (prev == nullptr)
        {
            curr = utils::exchange(sleepingHead, thread);
        }
        else
        {
            prev->next = thread;
        }
        thread->next = curr;

        thread->setSleepingTime(ticks);
        if (curr != nullptr)
        {
            auto oldTicks = curr->getSleepingTime();
            curr->setSleepingTime(oldTicks - ticks);
        }
        Thread::dispatch();
    }

    void Scheduler::tick()
    {
        if (sleepingHead == nullptr) return;
        sleepingHead->tick();
        awaken();
    }

    void Scheduler::awaken()
    {
        while (sleepingHead != nullptr && sleepingHead->getSleepingTime() == 0)
        {
            auto awake = utils::exchange(
                sleepingHead,
                sleepingHead->next);
            awake->next = nullptr;
            put(awake);
        }
    }

    Thread* Scheduler::getIdleThread()
    {
        if (idleThread == nullptr)
        {
            idleThread = new Thread(
                [](void* arg) { while (true); },
                nullptr,
                kmalloc(DEFAULT_STACK_SIZE >> 3),
                Thread::Mode::SYSTEM);
        }
        return idleThread;
    }
}
