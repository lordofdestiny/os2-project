//
// Created by os on 7/16/22.
//

#include "../../h/kernel/Scheduler.h"
#include "../../h/kernel/MemoryAllocator.h"
#include "../../h/kernel/ConsoleUtils.h"

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
        wakeUpThreads();
    }

    void Scheduler::wakeUpThreads() {
        while(sleepingHead != nullptr && sleepingHead->getSleepingTime() == 0) {
            auto awake = sleepingHead;
            sleepingHead = sleepingHead->getNext();
            awake->setNext(nullptr);
            instance.put(awake);
        }
    }

    Thread* Scheduler::getIdleThread() {
        if(!idleThread) {
            auto& allocator = MemoryAllocator::getInstance();
            void* stack = allocator.allocateBytes(DEFAULT_STACK_SIZE);
            auto task = [](void* arg) { while(true); };
            idleThread = new Thread(task, nullptr,
                                    stack, Thread::Mode::SYSTEM);
        }
        return idleThread;
    }
}
