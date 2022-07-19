//
// Created by os on 7/16/22.
//

#ifndef PROJECT_TCB_H
#define PROJECT_TCB_H
#include "../../lib/hw.h"

namespace kernel {
    class TCB {
    public:
        using ThreadTask = void(*)(void*);

        enum class ThreadType {
            USER, SYSTEM
        };

        enum class ThreadStatus {
            CREATED, READY, RUNNING, SLEEPING
        };

        friend class Scheduler;
        friend class SystemCalls;

        struct Registers{
            uint64 zero, ra, sp, gp, tp, t0, t1, t2,
                    s0, s1, a0, a1, a2, a3, a4, a5,
                    a6, a7, s2, s3, s4, s5, s6, s7,
                    s8, s9, s10, s11, t3, t4, t5, t6;
            Registers();
        };
    public:
        static void initializeMainThread();

        static void tick();
        static void dispatch();

        static TCB* getRunningThread();

        static void* operator new(size_t size);
        static void operator delete(void* ptr) noexcept;
    private:
        static void taskWrapper();
        static uint64 sstatusGetInitial();
        static uint64* pcGetInitial(ThreadTask function);
        static ThreadType runningThreadType();

        static TCB* mainThread;
        static TCB* runningThread;
        static time_t runningTimeLeft;

        static uint64 threadIdSource;
    private:
        struct ThreadContext {
            uint64* programCounter = nullptr;
            uint64 sstatus;
            Registers registers;
            explicit ThreadContext(uint64 status = 0, uint64* pc = nullptr);
        } context;
        ThreadTask task;
        void* arg;
        uint64* stack;
        ThreadType type;
        TCB* next = nullptr;
        uint64 id = threadIdSource++;
        ThreadStatus status = ThreadStatus::READY;
    public:
        TCB(ThreadTask function, void* argument, void* stack);
        TCB(ThreadTask function, void* argument, void* stack, ThreadType type);
        TCB(TCB const&)=delete;
        TCB& operator=(TCB const&)=delete;
        ~TCB();

        uint64 getThreadId() const;
        Registers& getRegisters();
        uint64* getPC() const;
        void setPC(uint64* value);
        uint64 getsstatus() const ;
        void setStatus(ThreadStatus newStatus);
        ThreadStatus getStatus();
        bool isUserThread() const;
    };

} // kernel

#endif //PROJECT_TCB_H
