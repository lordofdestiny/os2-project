//
// Created by os on 7/16/22.
//

#include "../../h/kernel/Scheduler.h"
#include "../../h/kernel/MemoryAllocator.h"

namespace kernel {
    Scheduler Scheduler::instance{};

    Scheduler& Scheduler::getInstance() {
        return instance;
    }

    void Scheduler::put(kernel::Thread *thread) {
        if(thread == idleThread) return;

        thread->setStatus(Thread::Status::READY);
        if(readyHead == nullptr) {
            readyHead = thread;
        }else {
            readyTail->setNext(thread);
        }
        readyTail = thread;
    }

    Thread *Scheduler::get() {
        if(readyHead == nullptr) return getIdleThread();

        auto thread = readyHead;
        readyHead = readyHead->getNext();
        if(readyHead == nullptr) {
            readyTail = nullptr;
        }
        thread->setNext(nullptr);
        return thread;
    }

    void Scheduler::putToSleep(Thread *thread, uint64 ticks) {

    }

    void Scheduler::tick() {

    }

    void Scheduler::wake() {

    }

    Thread* Scheduler::getIdleThread() {
        if(!idleThread) {
            auto& allocator = MemoryAllocator::getInstance();
            void* stack = allocator.allocateBytes(DEFAULT_STACK_SIZE);
            auto task = [](void*) {
                while(true);
            };
            idleThread = new Thread(task, nullptr, stack, Thread::Owner::SYSTEM);
        }
        return idleThread;
    }
}
