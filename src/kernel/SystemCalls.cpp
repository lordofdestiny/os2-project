//
// Created by os on 7/16/22.
//

#include "../../h/kernel/SystemCalls.h"
#include "../../h/kernel/MemoryAllocator.h"
#include "../../h/kernel/TCB.h"
#include "../../h/kernel/Scheduler.h"
#include "../../h/syscall_c.h"
#include "../../h/kernel/TrapHandlers.h"
#include "../../h/kernel/Semaphore.h"

#define RETURN_IF(test, value)        \
    do{                             \
        if((test)) {                \
            registers.a0 = value;   \
            return;                 \
        }                           \
    }while(0)                       \

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
    auto thread = new TCB(task, argument, stack);
    RETURN_IF(thread == nullptr, -0x01);
    *handle = (thread_t) thread;
    Scheduler::getInstance().put(thread);
    registers.a0 = 0x00;
}

void kernel::SystemCalls::thread_exit() { // Handle if attempting to exit main
    auto runningThread = TCB::getRunningThread();
    auto &registers = runningThread->getRegisters();
    RETURN_IF(runningThread == TCB::getMainThread(), -0x01);
    delete runningThread;
    TCB::dispatch();
    registers.a0 = 0x00;
}

void kernel::SystemCalls::sem_open() {
    auto &registers = TCB::getRunningThread()->getRegisters();
    auto init = (unsigned ) registers.a2;
    auto handle = (sem_t *) registers.a1;
    auto semaphore = new Semaphore((int) init);
    RETURN_IF(semaphore == nullptr, -0x01);
    *handle = (sem_t) semaphore;
    registers.a0 = 0x00;
}

void kernel::SystemCalls::sem_close() {
    auto &registers = TCB::getRunningThread()->getRegisters();
    auto handle = (Semaphore*) registers.a1;
    RETURN_IF(handle == nullptr, -0x01);
    delete handle;
    registers.a0 = 0x00;
}

void kernel::SystemCalls::sem_wait() {
    auto &registers = TCB::getRunningThread()->getRegisters();
    auto id = (Semaphore*) registers.a1;
    RETURN_IF(id == nullptr, -0x01);
    registers.a0 = 0x00;
    id->wait();
}

void kernel::SystemCalls::sem_signal() {
    auto &registers = TCB::getRunningThread()->getRegisters();
    auto id = (Semaphore*) registers.a1;
    RETURN_IF(id == nullptr, -0x01);
    registers.a0 = 0x00;
    id->signal();
}