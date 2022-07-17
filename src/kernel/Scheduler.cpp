//
// Created by os on 7/16/22.
//

#include "../../h/kernel/Scheduler.h"
#include "../../h/kernel/MemoryAllocator.h"

namespace kernel {

    Scheduler& kernel::Scheduler::getInstance() {
        static Scheduler instance{};
        return instance;
    }

    TCB *kernel::Scheduler::get() {
        if(readyHead == nullptr)return getIdleThread();

        auto result = readyHead;
            readyHead = readyHead->next;
            if(readyHead == nullptr) {
                readyTail = nullptr;
            }
            result->next= nullptr;

        return result;
    }

    void Scheduler::put(kernel::TCB *thread) {
        if(thread == idleThread) return;

        thread->status = TCB::ThreadStatus::READY;
        if(readyHead == nullptr) {
            readyHead = thread;
        }else {
            readyTail->next = thread;
        }
        readyTail = thread;

    }

    TCB* Scheduler::getIdleThread() {
        if(!idleThread) {
            void* stack = MemoryAllocator::getInstance().allocateBytes(DEFAULT_STACK_SIZE);
            idleThread = new TCB([](void*){while(true);}, nullptr,stack);
        }
        return idleThread;
    }

    Scheduler::~Scheduler() {
        if(idleThread != nullptr) {
            // Delete idle thread
        }
    }
}
