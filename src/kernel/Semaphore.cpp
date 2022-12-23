//
// Created by os on 7/18/22.
//

#include "../../h/kernel/Semaphore.h"
#include "../../h/kernel/Scheduler.h"
#include "../../h/kernel/Memory/HeapAllocator.h"

namespace kernel {

    void* Semaphore::operator new(size_t size) noexcept {
        return ALLOCATOR.allocateBytes(size);
    }

    void Semaphore::operator delete(void *ptr) noexcept {
        ALLOCATOR.deallocateBlocks(ptr);
    }

    Semaphore::Semaphore(int value) :
        value(value) { }

    Semaphore::~Semaphore() {
        auto curr = head;
        while(curr != nullptr) {
            curr->getContext().getRegisters().a0 = -0x02;
            SCHEDULER.put(curr);
            curr=curr->getNext();
        }
    }

    void Semaphore::wait() {
        if(--value < 0) block();
    }

    void Semaphore::signal() {
        if(++value<=0) unblock();
    }

    void Semaphore::block() {
        auto thread = Thread::getRunning();
        Thread::shelveRunning();
        thread->setStatus(Thread::Status::BLOCKED);
        enqueue(thread);
        Thread::dispatch();
    }

    void Semaphore::unblock() {
        auto thread = dequeue();
        if(thread == nullptr) return;
        SCHEDULER.put(thread);
    }

    void Semaphore::enqueue(Thread *thread) {
        if(head == nullptr){
            head = thread;
        }else{
            tail->setNext(thread);
        }
        tail = thread;
    }

    Thread *Semaphore::dequeue() {
        if(head == nullptr) return nullptr;
        auto thread = head;
        head = head->getNext();
        if(head == nullptr) tail = nullptr;
        thread->setNext(nullptr);
        return thread;
    }
} // kernel