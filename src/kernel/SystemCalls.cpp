//
// Created by os on 7/16/22.
//

#include "../../h/kernel/SystemCalls.h"
#include "../../h/kernel/MemoryAllocator.h"
#include "../../h/kernel/TCB.h"
#include "../../h/kernel/Scheduler.h"
#include "../../h/syscall_c.h"
#include "../../h/kernel/TrapHandler.h"


void kernel::SystemCalls::mem_alloc() {
    auto &registers = TCB::getRunningThread()->getRegisters();
    size_t blockCount = registers.a1;
    auto memory = MemoryAllocator::getInstance().allocateBlocks(blockCount);
    registers.a0 = (uint64) memory;
}

void kernel::SystemCalls::mem_free() {
    auto &registers = TCB::getRunningThread()->getRegisters();
    auto memory = (void *) registers.a1;
    int code = MemoryAllocator::getInstance().deallocateBlocks(memory);
    registers.a0 = code;
}

void kernel::SystemCalls::thread_create() {
    auto &registers = TCB::getRunningThread()->getRegisters();

    auto handle = (thread_t *) registers.a1;
    auto task = (TCB::ThreadTask) registers.a2;
    auto argument = (void *) registers.a3;
    auto stack = (void *) registers.a4;

    *handle = (thread_t) new TCB(task, argument, stack);
    if (*handle) {
        Scheduler::getInstance().put((TCB *) *handle);
        registers.a0 = 0x00;
    } else {
        registers.a0 = -0x01;
    }
}

void kernel::SystemCalls::thread_exit() { // Handle if attempting to exit main
    auto &registers = TCB::runningThread->getRegisters();
    if(TCB::runningThread == TCB::mainThread){
        registers.a0 = -0x01;
    }else{
        delete TCB::runningThread;
        registers.a1 = -0x01;
    }
}

void kernel::SystemCalls::handle() {
    kernel::TrapHandler::incrementPC();

    auto type = (Type) TCB::getRunningThread()->getRegisters().a0;

    switch (type) {
        case Type::MemoryAllocate:
            return mem_alloc();
        case Type::MemoryFree:
            return mem_free();
        case Type::ThreadCreate:
            return thread_create();
        case Type::ThreadExit:
            return thread_exit();
        case Type::ThreadDispatch:
            return TCB::dispatch();
        case Type::SemaphoreOpen:
            break;
        case Type::SemaphoreClose:
            break;
        case Type::SemaphoreWait:
            break;
        case Type::SemaphoreSignal:
            break;
        case Type::TimeSleep:
            break;
        case Type::GetChar:
            break;
        case Type::PutChar:
            break;
    }

}
