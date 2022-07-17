//
// Created by os on 7/16/22.
//

#include "../../h/kernel/TCB.h"
#include "../../h/kernel/Scheduler.h"
#include "../../h/kernel/MemoryAllocator.h"

namespace kernel {
    TCB* TCB::runningThread;
    time_t TCB::runningTimeLeft;
    TCB* TCB::mainThread;
    uint64 TCB::threadIdSource = 0;

    TCB::TCB(Task function, void *argument, void *stack) :
        task(function), arg(argument),
        stack((size_t*) stack) {
        auto stackTop = (uint64) &this->stack[DEFAULT_STACK_SIZE];
        context.registers.sp = stackTop;
    }

    TCB::ThreadContext::ThreadContext() :
        registers(),
        programCounter((uint64*) &taskWrapper){ }

    TCB::Registers::Registers() {
        for(int i = 0; i < 32; i++){
            ((uint64*)this)[i] = 0;
        }
    }

    void TCB::taskWrapper() {
        runningThread->task(runningThread->arg);
        // kill thread
    }

    void* TCB::operator new(size_t size) {
        auto blocks = MemoryAllocator::byteSizeToBlockCount(size);
        return MemoryAllocator::getInstance().allocateBlocks(blocks);
    }

    void TCB::operator delete(void *ptr) noexcept {
        MemoryAllocator::getInstance().deallocateBlocks(ptr);
    }

    TCB *TCB::getRunningThread() {
        return runningThread;
    }

    TCB::~TCB() {
        if(stack != nullptr) {
            MemoryAllocator::getInstance().deallocateBlocks(stack);
        }
        if(this == runningThread){
            runningThread = nullptr;
            dispatch();
        }
    }

    void TCB::initializeMainThread() {
        mainThread = new TCB(nullptr, nullptr, nullptr);
        runningThread = mainThread;
        runningTimeLeft = DEFAULT_TIME_SLICE;
    }

    void TCB::tick() {
        return;
    }

    void TCB::dispatch() {
        auto& scheduler = Scheduler::getInstance();

        auto oldThread = runningThread;

        if (oldThread && oldThread->status != ThreadStatus::SLEEPING) {
            scheduler.put(oldThread);
        }

        auto newThread = scheduler.get();

        runningThread = newThread;
        runningThread->status = ThreadStatus::RUNNING;
        runningTimeLeft = DEFAULT_TIME_SLICE;
    }
} // kernel