//
// Created by os on 7/16/22.
//

#include "../../h/kernel/MemoryAllocator.h"
#include "../../h/kernel/Thread.h"
#include "../../h/kernel/Scheduler.h"
#include "../../h/syscall_c.h"
#include "../../h/kernel/Semaphore.h"

#define RETURN_IF(test, value)        \
    do{                             \
        if((test)) {                \
            registers.a0 = value;   \
            return;                 \
        }                           \
    }while(0)                       \

namespace kernel {
    namespace SystemCalls {
        Thread::Context::Registers& threadRegisters() {
            return Thread::getRunning()->getContext().getRegisters();
        }

        void mem_alloc() {
            auto &registers = threadRegisters();
            size_t blockCount = registers.a1;
            auto memory = MemoryAllocator::getInstance().allocateBlocks(blockCount);
            registers.a0 = (uint64) memory;
        }

        void mem_free() {
            auto &registers = threadRegisters();
            auto memory = (void *) registers.a1;
            int code = MemoryAllocator::getInstance().deallocateBlocks(memory);
            registers.a0 = code;
        }

        void thread_create() {
            auto &registers = threadRegisters();

            auto handle = (thread_t *) registers.a1;
            auto task = (Thread::Task) registers.a2;
            auto argument = (void *) registers.a3;
            auto stack = (void *) registers.a4;
            auto thread = new Thread(task, argument, stack);
            RETURN_IF(thread == nullptr, -0x01);
            *handle = (thread_t) thread;
            Scheduler::getInstance().put(thread);
            registers.a0 = 0x00;
        }

        void thread_exit() { // Handle if attempting to exit main
            auto runningThread = Thread::getRunning();
            auto &registers = threadRegisters();
            RETURN_IF(runningThread == Thread::getMainThread(), -0x01);
            delete runningThread;
            Thread::dispatch();
            registers.a0 = 0x00;
        }

        void sem_open() {
            auto &registers = threadRegisters();
            auto init = (unsigned ) registers.a2;
            auto handle = (sem_t *) registers.a1;
            auto semaphore = new Semaphore((int) init);
            RETURN_IF(semaphore == nullptr, -0x01);
            *handle = (sem_t) semaphore;
            registers.a0 = 0x00;
        }

        void sem_close() {
            auto &registers = threadRegisters();
            auto handle = (Semaphore*) registers.a1;
            RETURN_IF(handle == nullptr, -0x01);
            delete handle;
            registers.a0 = 0x00;
        }

        void sem_wait() {
            auto &registers = threadRegisters();
            auto id = (Semaphore*) registers.a1;
            RETURN_IF(id == nullptr, -0x01);
            registers.a0 = 0x00;
            id->wait();
        }

        void sem_signal() {
            auto &registers = threadRegisters();
            auto id = (Semaphore*) registers.a1;
            RETURN_IF(id == nullptr, -0x01);
            registers.a0 = 0x00;
            id->signal();
        }

        void time_sleep() {
            auto& registers = threadRegisters();
            auto& scheduler = Scheduler::getInstance();
            auto thread = Thread::getRunning();
            auto ticks = registers.a1;
            RETURN_IF(ticks < 0, -0x01);
            registers.a0 = 0x00;
            scheduler.putToSleep(thread, ticks);
        }
    }
}