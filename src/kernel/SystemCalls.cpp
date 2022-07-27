//
// Created by os on 7/16/22.
//
#include "../../h/kernel/SystemCalls.h"
#include "../../h/kernel/MemoryAllocator.h"
#include "../../h/kernel/Thread.h"
#include "../../h/kernel/Scheduler.h"
#include "../../h/syscall_c.h"
#include "../../h/kernel/Semaphore.h"
#include "../../h/kernel/Console.h"

#define RETURN_IF(test, value)      \
    do{                             \
        if((test)) {                \
            RETURN(value);          \
            return;                 \
        }                           \
    }while(0)                       \

namespace kernel {
    namespace SystemCalls {
        void mem_alloc() {
            auto blockCount = ACCEPT(size_t, 1);
            auto memory = ALLOCATOR.allocateBlocks(blockCount);
            RETURN(memory);
        }

        void mem_free() {
            auto memory = ACCEPT(void*, 1);
            int code = ALLOCATOR.deallocateBlocks(memory);
            RETURN(code);
        }

        void thread_create() {
            thread_init();
            auto handle = ACCEPT(thread_t*, 1);
            auto thread = (Thread*) *handle;
            RETURN_IF(thread == nullptr, -0x02);
            thread->setStatus(Thread::Status::READY);
            thread_start();
        }

        void thread_exit() { // Handle if attempting to exit main
            auto runningThread = Thread::getRunning();
            RETURN_IF(runningThread == Thread::getMainThread(), -0x01);
            delete runningThread;
            Thread::dispatch();
            RETURN(0);
        }

        void thread_init() {
            auto handle = ACCEPT(thread_t*, 1);
            RETURN_IF(handle == nullptr, -0x01);
            auto task = ACCEPT(Thread::Task, 2);
            auto argument = ACCEPT(void*, 3);
            auto stack = ACCEPT(void*, 4);
            auto mode = Thread::getMode(Thread::getRunning());
            auto thread = new Thread(task, argument, stack, mode);
            if(thread == nullptr) {
                ALLOCATOR.deallocateBlocks(stack);
            }else{
                *handle = (thread_t) thread;
            }
            RETURN_IF(thread == nullptr, -0x01);
            RETURN(0x00);
        }

        void thread_start() {
            auto thread = (Thread*) *ACCEPT(thread_t*, 1);
            switch (thread->getStatus()) {
                case Thread::Status::CREATED:
                    thread->setStatus(Thread::Status::READY);
                case Thread::Status::READY:
                    return SCHEDULER.put(thread);
                case Thread::Status::RUNNING:
                case Thread::Status::BLOCKED:
                    return;
            }
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
            RETURN(0);
            id->signal();
        }

        void time_sleep() {
            auto thread = Thread::getRunning();
            auto ticks = ACCEPT(time_t, 1);
            RETURN_IF(ticks < 0, -0x01);
            RETURN(0);
            SCHEDULER.entrance(thread, ticks);
        }

        void getc() {
            auto c = CONSOLE.readChar();
            RETURN(c);
        }

        void putc() {
            auto c = ACCEPT(char, 1);
            CONSOLE.writeChar(c);
        }

        void enter_user_mode() {
            auto thread = Thread::getRunning();
            thread->enterUserMode();
        }
    }
}