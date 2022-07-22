//
// Created by os on 7/16/22.
//

#include "../../h/kernel/MemoryAllocator.h"
#include "../../h/kernel/Thread.h"
#include "../../h/kernel/Scheduler.h"
#include "../../h/syscall_c.h"
#include "../../h/kernel/Semaphore.h"
#include "../../h/kernel/Console.h"

#define ACCEPT(type, index) (type) threadRegisters().a##index
#define RETURN(value) threadRegisters().a0 = (uint64) value

#define RETURN_IF(test, value)      \
    do{                             \
        if((test)) {                \
            RETURN(value);          \
            return;                 \
        }                           \
    }while(0)                       \

namespace kernel {
    namespace SystemCalls {
        Thread::Context::Registers& threadRegisters() {
            return Thread::getRunning()->getContext().getRegisters();
        }

        void mem_alloc() {
            auto blockCount = ACCEPT(size_t, 1);
            auto memory = MemoryAllocator::getInstance().allocateBlocks(blockCount);
            RETURN(memory);
        }

        void mem_free() {
            auto memory = ACCEPT(void*, 1);
            int code = MemoryAllocator::getInstance().deallocateBlocks(memory);
            RETURN(code);
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
            RETURN_IF(runningThread == Thread::getMainThread(), -0x01);
            delete runningThread;
            Thread::dispatch();
            registers.a0 = 0x00;
        }

        void sem_open() {
            auto handle = ACCEPT(sem_t*, 1);
            auto init = ACCEPT(unsigned , 2);
            auto semaphore = new Semaphore((int) init);
            RETURN_IF(semaphore == nullptr, -0x01);
            *handle = (sem_t) semaphore;
            RETURN(0);
        }

        void sem_close() {
            auto handle = ACCEPT(Semaphore*, 1);
            RETURN_IF(handle == nullptr, -0x01);
            delete handle;
            RETURN(0);
        }

        void sem_wait() {
            auto id = ACCEPT(Semaphore*, 1);
            RETURN_IF(id == nullptr, -0x01);
            RETURN(0);
            id->wait();
        }

        void sem_signal() {
            auto id = ACCEPT(Semaphore*, 1);
            RETURN_IF(id == nullptr, -0x01);
            registers.a0 = 0x00;
            id->signal();
        }

        void time_sleep() {
            auto& scheduler = Scheduler::getInstance();
            auto thread = Thread::getRunning();
            auto ticks = ACCEPT(time_t, 1);
            RETURN_IF(ticks < 0, -0x01);
            RETURN(0);
            scheduler.putToSleep(thread, ticks);
        }

        void getc() {
            auto& console = Console::getInstance();
            auto c = console.readChar();
            RETURN(c);
        }

        void putc() {
            char c = threadRegisters().a1;
            auto& console = Console::getInstance();
            console.writeChar(c);
        }
    }
}