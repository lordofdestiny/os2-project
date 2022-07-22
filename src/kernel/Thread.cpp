//
// Created by os on 7/16/22.
//

#include "../../h/kernel/Thread.h"
#include "../../h/kernel/Scheduler.h"
#include "../../h/kernel/MemoryAllocator.h"
#include "../../h/kernel/BitMasks.h"
#include "../../h/syscall_c.h"

namespace kernel {
    Thread* Thread::mainThread = nullptr;
    Thread* Thread::runningThread = nullptr;
    time_t Thread::runningTimeLeft = DEFAULT_TIME_SLICE;
    uint64 Thread::threadIdSource = 0;
    uint64 Thread::userThreadCount = 0;
    uint64 Thread::systemThreadCount = 0;

    Thread::Context::Context(uint64 pc, uint64 status) :
            pc(pc), sstatus(status), registers(){ }

    Thread::Context::Registers::Registers() {
        for(int i = 0; i < 32; i++){
            ((uint64*)this)[i] = 0;
        }
    }

    void* Thread::operator new(size_t size) noexcept {
        auto blocks = MemoryAllocator::byteSizeToBlockCount(size);
        return MemoryAllocator::getInstance().allocateBlocks(blocks);
    }

    void Thread::operator delete(void *ptr) noexcept {
        MemoryAllocator::getInstance().deallocateBlocks(ptr);
    }

    void Thread::initialize() {
        runningThread = getMainThread();
        runningTimeLeft = DEFAULT_TIME_SLICE;
    }

    void Thread::dispatch() {
        auto& scheduler = Scheduler::getInstance();

        auto oldThread = runningThread;

        if (oldThread && oldThread->status != Status::BLOCKED) {
            scheduler.put(oldThread);
        }

        auto newThread = scheduler.get();

        runningThread = newThread;
        runningThread->status = Status::RUNNING;
        runningTimeLeft = DEFAULT_TIME_SLICE;
    }

    Thread* Thread::getMainThread() {
        if(mainThread == nullptr) {
            mainThread = new Thread(nullptr, nullptr, nullptr, Owner::USER);
        }
        return mainThread;
    }

    Thread* Thread::getRunning() {
        return runningThread;
    }

    void Thread::shelveRunning() {
        runningThread = nullptr;
    }

    uint64 Thread::threadCount(Thread::Mode owner) {
        if (owner == Mode::USER) {
            return userThreadCount;
        } else {
            return systemThreadCount;
        }
    }

    void Thread::taskWrapper() {
        runningThread->task(runningThread->arg);
        thread_exit();
    }

    uint64 Thread::sstatusGetInitial() {
        if(runningThread == nullptr) return 0;
        using BitMasks::sstatus;
        auto mask = (uint64)sstatus::SPP | (uint64) sstatus::SPIE;
        return runningThread->getContext().getsstatus() & mask;
    }

    uint64 Thread::pcGetInitial(Task function) {
        if(function == nullptr) return 0x00;
        return (uint64) taskWrapper;
    }

    /* Return to old version once user mode is the default mode */
    Thread::Owner Thread::runningThreadOwner() { // Try and replace with runningTrhread->owner
        //auto status = runningThread->getsstatus();
        //return status | (uint64) BitMasks::sstatus::SPP? Owner::SYSTEM:Owner::USER;
        return Owner::USER;
    }

    /* Return to a single constructore once user mode is the default mode */
    Thread::Thread(Task function, void *argument, void *stack) :
            Thread::Thread(function, argument, stack, runningThreadOwner()){ }

    Thread::Thread(Task function, void *argument, void *stack, Owner type) :
            context( pcGetInitial(function), sstatusGetInitial()),
            task(function), arg(argument), stack((size_t*) stack),
            owner(type) {
        if(stack != nullptr) {
            auto stackTop = (uint64) &this->stack[DEFAULT_STACK_SIZE];
            context.registers.sp = stackTop;
        }

        if(owner == Owner::USER) {
            userThreadCount++;
        }else if(owner == Owner::SYSTEM) {
            systemThreadCount++;
        }

    }

    Thread::~Thread() {
        if(stack != nullptr) {
            auto& allocator = MemoryAllocator::getInstance();
            allocator.deallocateBlocks(stack);
        }
        if(owner == Owner::USER) {
            userThreadCount--;
        }else if(owner == Owner::SYSTEM) {
            systemThreadCount--;
        }
        if(this == runningThread) runningThread = nullptr;
    }

    void Thread::tick() {
        if(this == runningThread && --runningTimeLeft == 0) {
            dispatch();
        }else {
            sleepingTime--;
        }
    }
} // kernel