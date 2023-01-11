//
// Created by os on 7/16/22.
//

#ifndef PROJECT_THREAD_H
#define PROJECT_THREAD_H
#include "../../lib/hw.h"
#include "./Memory/slab.h"
#include "./Semaphore.h"
#include "./Scheduler.h"
#include "../syscall_c.h"

#define CONTEXT Thread::getRunning()->getContext()
#define RUNNING_REGISTERS CONTEXT.getRegisters()
#define NEXT_INSTRUCTION() Thread::getRunning()->skipInstruction();

namespace kernel
{
    class Thread final
    {
    public:
        using Task = void(*)(void*);

        enum class Mode { USER, SYSTEM };

        enum class Status
        {
            CREATED, READY, RUNNING, BLOCKED, COMPLETE
        };

        class Context
        {
        public:
            friend Thread;
            struct Registers
            {
                uint64 zero, ra, sp, gp, tp, t0, t1, t2,
                    s0, s1, a0, a1, a2, a3, a4, a5,
                    a6, a7, s2, s3, s4, s5, s6, s7,
                    s8, s9, s10, s11, t3, t4, t5, t6;
                Registers();
            };
        private:
            uint64 pc = 0x00;
            uint64 sstatus = 0x00;
            Registers registers;
        public:
            explicit Context(uint64 pc = 0x00, uint64 status = 0);
            Registers& getRegisters() { return registers; }

            uint64 getPC() const { return pc; }
            void setPC(uint64 value) { pc = value; }

            uint64 getsstatus() const { return sstatus; }
            void setsstatus(uint64 value) { sstatus = value; }
        };
    public:
        friend class Semaphore;
        friend class Scheduler;

        static void* operator new(size_t size) noexcept;
        static void operator delete(void* ptr) noexcept;

        static void initialize();

        static void dispatch();

        static Thread* getMainThread();
        static void setMainFinished();
        static bool isMainFinished();

        static Thread* getRunning();
        static void shelveRunning();
        static uint64 getCount(Mode mode);

    private:
        static void taskWrapper();

        static uint64 sstatusGetInitial(Mode mode);
        static uint64 pcGetInitial(Task function);

        static kmem_cache_t* object_cache;
        static Thread* mainThread;
        static Thread* runningThread asm("__runningThread");
        static time_t runningTimeLeft;

        static bool mainFinished;
        static uint64 threadIdSource;
        static uint64 threadCounter[2];

        static Thread* createdHead;
    private:
        Context context;
        Task task;
        void* arg;
        uint64* stack;
        Mode mode;
        uint64 id = threadIdSource++;
        Status status = Status::CREATED;
        uint64 sleepingTime = 0;
        Thread* next = nullptr;
        Thread* prev = nullptr;
    public:
        Thread(Task function, void* argument, void* stack, Mode mode);
        Thread(Thread const&) = delete;
        Thread& operator=(Thread const&) = delete;
        ~Thread();

        void skipInstruction() { context.pc += 4; }

        Context& getContext() { return context; }

        Mode getMode() const { return mode; }

        uint64 getId() const { return id; }

        Status getStatus() { return status; };
        void setStatus(Status value) { status = value; }

        uint64 getSleepingTime() const { return sleepingTime; }
        void setSleepingTime(uint64 time) { sleepingTime = time; }
        void tick();
        void enterUserMode();
    };
} // kernel

#endif //PROJECT_THREAD_H
