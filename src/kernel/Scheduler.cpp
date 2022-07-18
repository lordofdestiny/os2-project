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
        if(readyHead == nullptr) return getIdleThread();

        auto thread = readyHead;
        readyHead = readyHead->next;
        if(readyHead == nullptr) {
            readyTail = nullptr;
        }
        thread->next= nullptr;
        if(thread->isUserThread()){
            userThreadCount--;
        }

        return thread;
    }

    void Scheduler::put(kernel::TCB *thread) {
        if(thread == idleThread) return;

        thread->status = TCB::ThreadStatus::READY;
        if(readyHead == nullptr) {
            readyHead = thread;
        }else {
            readyTail->next = thread;
        }
        if(thread->isUserThread()){
            userThreadCount++;
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

    bool Scheduler::hasUserThreads() {
        return userThreadCount != 0;
    }

//    Scheduler::~Scheduler() {
//        if(idleThread != nullptr) {
//            // Delete idle thread
//        }
//    }
}
