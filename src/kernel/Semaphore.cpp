//
// Created by os on 7/18/22.
//

#include "../../h/kernel/Semaphore.h"
#include "../../h/kernel/Scheduler.h"
#include "../../h/kernel/MemoryAllocator.h"
#include "../../h/kernel/ConsoleUtils.h"

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
            curr->getContext().getRegisters().a0 = -0x02;
            scheduler.put(curr);
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
        Scheduler::getInstance().put(thread);
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