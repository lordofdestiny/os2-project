//
// Created by os on 7/16/22.
//

#include "../../h/kernel/TCB.h"
#include "../../h/kernel/Scheduler.h"
#include "../../h/kernel/MemoryAllocator.h"
#include "../../h/kernel/BitMasks.h"
#include "../../h/syscall_c.h"

namespace kernel {
    TCB* TCB::runningThread;
    time_t TCB::runningTimeLeft;
    TCB* TCB::mainThread;
    uint64 TCB::threadIdSource = 0;

    uint64 TCB::sstatusGetInitial() {
        if(runningThread == nullptr) return 0;
        auto mask = (uint64)BitMasks::sstatus::SPP | (uint64) BitMasks::sstatus::SPIE;
        return runningThread->getsstatus() & mask;
    }

    uint64* TCB::pcGetInitial(ThreadTask function) {
        if(function == nullptr) return nullptr;
        return (uint64*) taskWrapper;
    }

    /* Return to old version once user mode is the default mode */
    TCB::ThreadType TCB::runningThreadType() {
        //auto status = runningThread->getsstatus();
        //return status | (uint64) BitMasks::sstatus::SPP? ThreadType::SYSTEM:ThreadType::USER;
        return ThreadType::USER;
    }

    /* Return to a single constructore once user mode is the default mode */
    TCB::TCB(ThreadTask function, void *argument, void *stack) :
            TCB::TCB(function,argument,stack, runningThreadType()){ }

    TCB::TCB(ThreadTask function, void *argument, void *stack, ThreadType type) :
            context(sstatusGetInitial(), pcGetInitial(function)),
            task(function), arg(argument), stack((size_t*) stack),
            type(type) {
        if(stack != nullptr) {
            auto stackTop = (uint64) &this->stack[DEFAULT_STACK_SIZE];
            context.registers.sp = stackTop;
        }
    }

    TCB::ThreadContext::ThreadContext(uint64 status, uint64* pc) :
            programCounter(pc), sstatus(status), registers(){ }

    TCB::Registers::Registers() {
        for(int i = 0; i < 32; i++){
            ((uint64*)this)[i] = 0;
        }
    }

    void TCB::taskWrapper() {
        runningThread->task(runningThread->arg);
        thread_exit();
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

    uint64 TCB::getThreadId() const {
        return threadId;
    }

    TCB::Registers& TCB::getRegisters() {
        return context.registers;
    }

    uint64* TCB::getPC() const {
        return context.programCounter;
    }
    void TCB::setPC(uint64* value) {
        context.programCounter = value;
    }

    uint64 TCB::getsstatus() const {
        return context.sstatus;
    }
    void TCB::setStatus(ThreadStatus newStatus) {
        status = newStatus;
    }
    
    TCB::ThreadStatus TCB::getStatus() {
        return status;
    }

    bool TCB::isUserThread() const{
        return threadType == ThreadType::USER;
    }
} // kernel