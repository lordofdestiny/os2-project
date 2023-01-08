//
// Created by os on 7/18/22.
//

#include "../../h/kernel/Semaphore.h"
#include "../../h/kernel/Scheduler.h"
#include "../../h/kernel/Utils/Utils.h"

namespace kernel
{
    kmem_cache_t* Semaphore::object_cache = nullptr;
    void Semaphore::initialize()
    {
        object_cache =
            kmem_cache_create("sem_t",
                sizeof(Semaphore),
                nullptr, nullptr);
    }


    void* Semaphore::operator new(size_t size) noexcept
    {
        return kmem_cache_alloc(object_cache);
    }

    void Semaphore::operator delete(void* ptr) noexcept
    {
        kmem_cache_free(object_cache, ptr);
    }

    Semaphore::Semaphore(int value):
        value(value)
    { }

    Semaphore::~Semaphore()
    {
        auto curr = head;
        while (curr != nullptr)
        {
            curr->getContext()
                .getRegisters()
                .a0 = -0x02;
            SCHEDULER.put(curr);
            curr = curr->next;
        }
    }

    void Semaphore::wait()
    {
        if (--value < 0) block();
    }

    void Semaphore::signal()
    {
        if (++value <= 0) unblock();
    }

    void Semaphore::block()
    {
        auto thread = Thread::getRunning();
        Thread::shelveRunning();
        thread->setStatus(Thread::Status::BLOCKED);
        enqueue(thread);
        Thread::dispatch();
    }

    void Semaphore::unblock()
    {
        auto thread = dequeue();
        if (thread == nullptr) return;
        SCHEDULER.put(thread);
    }

    void Semaphore::enqueue(Thread* thread)
    {
        if (head == nullptr)
        {
            head = thread;
        }
        else
        {
            tail->next = thread;
        }
        tail = thread;
    }

    Thread* Semaphore::dequeue()
    {
        if (head == nullptr) return nullptr;
        auto thread = utils::exchange(head, head->next);
        if (head == nullptr) tail = nullptr;
        thread->next = nullptr;
        return thread;
    }
} // kernel