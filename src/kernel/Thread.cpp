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
    bool Thread::mainFinished = false;
    uint64 Thread::threadIdSource = 0;
    uint64 Thread::userThreadCount = 0;
    uint64 Thread::systemThreadCount = 0;

    Thread::Context::Context(uint64 pc, uint64 status) :
        pc(pc), sstatus(status), registers() {
    }

    Thread::Context::Registers::Registers() {
        for (int i = 0; i < 32; i++) {
            ((uint64*)this)[i] = 0;
        }
    }

    void* Thread::operator new(size_t size) noexcept {
        return ALLOCATOR.allocateBytes(size);
    }

    void Thread::operator delete(void* ptr) noexcept {
        ALLOCATOR.deallocateBlocks(ptr);
    }

    void Thread::initialize() {
        runningThread = getMainThread();
        runningTimeLeft = DEFAULT_TIME_SLICE;
    }

    void Thread::dispatch() {

        auto oldThread = runningThread;

        if (oldThread && oldThread->status != Status::BLOCKED) {
            SCHEDULER.put(oldThread);
        }

        auto newThread = SCHEDULER.get();

        runningThread = newThread;
        runningThread->status = Status::RUNNING;
        runningTimeLeft = DEFAULT_TIME_SLICE;
    }

    Thread* Thread::getMainThread() {
        if (mainThread == nullptr) {
            mainThread = new Thread(nullptr, nullptr,
                                    nullptr, Mode::SYSTEM);
        }
        return mainThread;
    }

    Thread* Thread::getRunning() {
        return runningThread;
    }

    void Thread::shelveRunning() {
        runningThread = nullptr;
    }

    uint64 Thread::getCount(Thread::Mode mode) {
        if (mode == Mode::USER) {
            return userThreadCount;
        } else {
            return systemThreadCount;
        }
    }

    void Thread::taskWrapper() {
        runningThread->task(runningThread->arg);
        thread_exit();
    }

    uint64 Thread::sstatusGetInitial(Mode mode) {
        using namespace BitMasks;
        using BitMasks::sstatus;
        auto status =
            runningThread == nullptr
            ? (uint64)sstatus::SPIE
            : runningThread->context.sstatus;
        auto SPP =
            mode == Mode::SYSTEM
            ? (uint64)sstatus::SPP
            : 0x00;
        auto SPIE = status & (uint64)sstatus::SPIE;
        return SPP | SPIE | (uint64)sstatus::SIE;
    }

    uint64 Thread::pcGetInitial(Task function) {
        if (function == nullptr) return 0x00;
        return (uint64)taskWrapper;
    }

    void Thread::setMainFinished() {
        mainFinished = true;
    }

    bool Thread::isMainFinished() {
        return mainFinished;
    }

    Thread::Thread(Task function, void* argument, void* stack, Mode mode) :
        context(pcGetInitial(function), sstatusGetInitial(mode)),
        task(function), arg(argument), stack((uint64*)stack), mode(mode) {
        if (stack != nullptr) {
            auto stackTop = (uint64)((char*)this->stack + DEFAULT_STACK_SIZE);
            context.registers.sp = stackTop;
        }

        if (mode == Mode::USER) {
            userThreadCount++;
        } else {
            systemThreadCount++;
        }
    }

    Thread::~Thread() {
        if (this->mode == Mode::USER) {
            userThreadCount--;
        } else {
            systemThreadCount--;
        }
        if (this == runningThread) runningThread = nullptr;
        if (stack != nullptr) {
            ALLOCATOR.deallocateBlocks(stack);
        }
    }

    void Thread::tick() {
        if (this == runningThread && --runningTimeLeft == 0) {
            dispatch();
        } else if (sleepingTime > 0) {
            sleepingTime--;
        }
    }

    void Thread::enterUserMode() {
        if (this->mode == Mode::SYSTEM) {
            using namespace BitMasks;
            auto newsstatus = context.sstatus & (~(uint64)sstatus::SPP);
            context.sstatus = newsstatus;

            mode = Mode::USER;

            systemThreadCount--;
            userThreadCount++;
        }
    }

} // kernel