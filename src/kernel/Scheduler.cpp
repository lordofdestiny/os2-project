//
// Created by os on 7/16/22.
//

#include "../../h/kernel/Scheduler.h"
#include "../../h/kernel/MemoryAllocator.h"

namespace kernel {
//    Scheduler Scheduler::instance{};

    Scheduler& Scheduler::getInstance() {
        static Scheduler instance{};
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

    void Scheduler::entrance(Thread *thread, uint64 ticks) {
        if(ticks == 0) return;

        thread->setStatus(Thread::Status::BLOCKED);

        Thread* prev = nullptr;
        Thread *curr = sleepingHead;
        while(curr != nullptr) {
            if(ticks <= curr->getSleepingTime()) {
                break;
            }
            ticks -= curr->getSleepingTime();
            prev = curr;
            curr = curr->getNext();
        }

        if(prev == nullptr) {
            curr = sleepingHead;
            sleepingHead = thread;
        } else {
            prev->setNext(thread);
        }
        thread->setNext(curr);

        thread->setSleepingTime(ticks);
        if(curr != nullptr) {
            auto oldTicks = curr->getSleepingTime();
            curr->setSleepingTime(oldTicks - ticks);
        }
        Thread::dispatch();
    }

    void Scheduler::tick() {
        if(sleepingHead == nullptr) return;
        sleepingHead->tick();
        awaken();
    }

    void Scheduler::awaken() {
        while(sleepingHead != nullptr && sleepingHead->getSleepingTime() == 0) {
            auto awake = sleepingHead;
            sleepingHead = sleepingHead->getNext();
            awake->setNext(nullptr);
            put(awake);
        }
    }

    Thread* Scheduler::getIdleThread() {
        if(idleThread == nullptr) {
            void* stack = ALLOCATOR.allocateBytes(DEFAULT_STACK_SIZE);
            auto task = [](void* arg) { while(true); };
            idleThread = new Thread(task, nullptr,
                                    stack, Thread::Mode::SYSTEM);
        }
        return idleThread;
    }
}
