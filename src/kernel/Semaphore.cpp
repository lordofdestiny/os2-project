//
// Created by os on 7/18/22.
//

#include "../../h/kernel/Semaphore.h"
#include "../../h/kernel/Scheduler.h"
#include "../../h/kernel/MemoryAllocator.h"

namespace kernel {

    void* Semaphore::operator new(size_t size) noexcept {
        auto blocks = MemoryAllocator::byteSizeToBlockCount(size);
        return MemoryAllocator::getInstance().allocateBlocks(blocks);
    }

    void Semaphore::operator delete(void *ptr) noexcept {
        MemoryAllocator::getInstance().deallocateBlocks(ptr);
    }

    Semaphore::Semaphore(int value) :
        value(value) { }

    Semaphore::~Semaphore() {
        auto& scheduler = Scheduler::getInstance();
        auto curr = head;
        while(curr != nullptr) {
            curr->getRegisters().a0 = -0x02;
            scheduler.put(curr);
            curr=curr->next;
        }
    }

    void Semaphore::wait() {
        if(--value < 0) block();
    }

    void Semaphore::signal() {
        if(++value<=0) unblock();
    }

    void Semaphore::block() {
        auto thread = TCB::getRunningThread();
        TCB::runningThread = nullptr;
        thread->setStatus(TCB::ThreadStatus::BLOCKED);
        enqueue(thread);
        TCB::dispatch();
    }

    void Semaphore::unblock() {
        auto thread = dequeue();
        if(thread == nullptr) return;
        Scheduler::getInstance().put(thread);
    }

    void Semaphore::enqueue(TCB *tcb) {
        if(head == nullptr){
            head = tcb;
        }else{
            tail->next = tcb;
        }
        tail = tcb;
    }

    TCB *Semaphore::dequeue() {
        if(head == nullptr) return nullptr;
        auto tcb = head;
        head = head->next;
        if(head == nullptr){
            tail = nullptr;
        }
        tcb->next= nullptr;
        return tcb;
    }
} // kernel