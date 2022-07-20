//
// Created by os on 7/16/22.
//

#include "../../h/kernel/Scheduler.h"
#include "../../h/kernel/MemoryAllocator.h"

namespace kernel {
    Scheduler& Scheduler::getInstance() {
        static Scheduler instance{};
        return instance;
    }

    Thread *Scheduler::get() {
        if(readyHead == nullptr) return getIdleThread();

        auto thread = readyHead;
        readyHead = readyHead->getNext();
        if(readyHead == nullptr) {
            readyTail = nullptr;
        }
        thread->setNext(nullptr);
        if(thread->isUserThread()){
            userThreadCount--;
        }

        return thread;
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

        if(thread->isUserThread()){
            userThreadCount++;
        }
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

    bool Scheduler::hasUserThreads() const{
        return userThreadCount != 0;
    }
}
