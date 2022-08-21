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

namespace kernel {
    SystemCallHandler SystemCalls::getHandler(CallType type) {
        auto group = resolveGroup(type);
        if (group == nullptr)
            return nullptr;
        return group->getCall(type);
    }
    SystemCalls& SystemCalls::getInstance() {
        static SystemCalls instance{};
        return instance;
    }
    SystemCallHandlerGroup* SystemCalls::resolveGroup(CallType type) {
        for (auto g : systemCallGroups) {
            if (g->handles(type)) return g;
        }
        return nullptr;
    }

    void SystemCalls::registerSystemCall(CallType type, SystemCallHandler hanlder) {
        auto group = resolveGroup(type);
        if (group == nullptr) {
            return;
        }
        group->registerCall(type, hanlder);
    }

    void SystemCalls::initialize() {
        systemCallGroups[0] = memoryCalls = new SystemCallHandlerGroup(SystemCallType::Memory);
        systemCallGroups[1] = threadCalls = new SystemCallHandlerGroup(SystemCallType::Thread);
        systemCallGroups[2] = semaphoreCalls = new SystemCallHandlerGroup(SystemCallType::Semaphore);
        systemCallGroups[3] = timerCalls = new SystemCallHandlerGroup(SystemCallType::Timer);
        systemCallGroups[4] = consoleCalls = new SystemCallHandlerGroup(SystemCallType::Console);
        systemCallGroups[5] = systemCalls = new SystemCallHandlerGroup(SystemCallType::System);
        registerSystemCall(CallType::MemoryAllocate, &mem_alloc);
        registerSystemCall(CallType::MemoryFree, &mem_free);
        registerSystemCall(CallType::ThreadCreate, &thread_create);
        registerSystemCall(CallType::ThreadExit, &thread_exit);
        registerSystemCall(CallType::ThreadDispatch, &thread_dispatch);
        registerSystemCall(CallType::ThreadInit, &thread_init);
        registerSystemCall(CallType::ThreadStart, &thread_start);
        registerSystemCall(CallType::SemaphoreOpen, &sem_open);
        registerSystemCall(CallType::SemaphoreClose, &sem_close);
        registerSystemCall(CallType::SemaphoreWait, &sem_wait);
        registerSystemCall(CallType::SemaphoreSignal, &sem_signal);
        registerSystemCall(CallType::TimeSleep, &time_sleep);
        registerSystemCall(CallType::GetChar, &getc);
        registerSystemCall(CallType::PutChar, &putc);
        registerSystemCall(CallType::EnterUserMode, &enter_user_mode);
    }

    SystemCalls::SystemCalls() = default;

    void SystemCalls::mem_alloc() {
        auto blockCount = ACCEPT(size_t, 1);
        auto memory = ALLOCATOR.allocateBlocks(blockCount);
        RETURN(memory);
    }

    void SystemCalls::mem_free() {
        auto memory = ACCEPT(void*, 1);
        int code = ALLOCATOR.deallocateBlocks(memory);
        RETURN(code);
    }

    void SystemCalls::thread_create() {
        thread_init();
        auto handle = ACCEPT(thread_t*, 1);
        auto thread = (Thread*)*handle;
        RETURN_IF(thread == nullptr, -0x02);
        thread->setStatus(Thread::Status::READY);
        thread_start();
    }

    void SystemCalls::thread_exit() { // Handle if attempting to exit main
        auto runningThread = Thread::getRunning();
        RETURN_IF(runningThread == Thread::getMainThread(), -0x01);
        delete runningThread;
        Thread::dispatch();
        RETURN(0);
    }

    void SystemCalls::thread_dispatch() {
        Thread::dispatch();
    }

    void SystemCalls::thread_init() {
        auto handle = ACCEPT(thread_t*, 1);
        RETURN_IF(handle == nullptr, -0x01);
        auto task = ACCEPT(Thread::Task, 2);
        auto argument = ACCEPT(void*, 3);
        auto stack = ACCEPT(void*, 4);
        auto mode = Thread::getMode(Thread::getRunning());
        auto thread = new Thread(task, argument, stack, mode);
        if (thread == nullptr) {
            ALLOCATOR.deallocateBlocks(stack);
        }
        else {
            *handle = (thread_t)thread;
        }
        RETURN_IF(thread == nullptr, -0x01);
        RETURN(0x00);
    }

    void SystemCalls::thread_start() {
        auto thread = (Thread*)*ACCEPT(thread_t*, 1);
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

    void SystemCalls::sem_open() {
        auto handle = ACCEPT(sem_t*, 1);
        auto init = ACCEPT(unsigned, 2);
        auto semaphore = new Semaphore((int)init);
        RETURN_IF(semaphore == nullptr, -0x01);
        *handle = (sem_t)semaphore;
        RETURN(0);
    }

    void SystemCalls::sem_close() {
        auto handle = ACCEPT(Semaphore*, 1);
        RETURN_IF(handle == nullptr, -0x01);
        delete handle;
        RETURN(0);
    }

    void SystemCalls::sem_wait() {
        auto id = ACCEPT(Semaphore*, 1);
        RETURN_IF(id == nullptr, -0x01);
        RETURN(0);
        id->wait();
    }

    void SystemCalls::sem_signal() {
        auto id = ACCEPT(Semaphore*, 1);
        RETURN_IF(id == nullptr, -0x01);
        RETURN(0);
        id->signal();
    }

    void SystemCalls::time_sleep() {
        auto thread = Thread::getRunning();
        auto ticks = ACCEPT(time_t, 1);
        RETURN_IF(ticks < 0, -0x01);
        RETURN(0);
        SCHEDULER.entrance(thread, ticks);
    }

    void SystemCalls::getc() {
        auto c = CONSOLE.readChar();
        if (c > 127) c = EOF;
        RETURN(c);
    }

    void SystemCalls::putc() {
        auto c = ACCEPT(char, 1);
        CONSOLE.writeChar(c);
    }

    void SystemCalls::enter_user_mode() {
        auto thread = Thread::getRunning();
        thread->enterUserMode();
    }

}