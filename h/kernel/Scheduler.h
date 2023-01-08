//
// Created by os on 7/16/22.
//

#ifndef PROJECT_SCHEDULER_H
#define PROJECT_SCHEDULER_H

#include "../../h/kernel/Thread.h"

#define SCHEDULER Scheduler::getInstance()

namespace kernel
{
    class Thread;

    class Scheduler final
    {
    public:
        friend class Kernel;
        static Scheduler& getInstance();

        Scheduler(Scheduler const&) = delete;
        Scheduler& operator=(Scheduler const&) = delete;
        ~Scheduler() = default;

        void put(Thread* thread);
        Thread* get();

        void entrance(Thread* thread, uint64 ticks);
        void tick();
    private:
        void awaken();
        Scheduler() = default;
        Thread* getIdleThread();

        static void initialize();
        static void* operator new(size_t size);
        static void operator delete(void* ptr);

    private:
        Thread* readyHead = nullptr;
        Thread* readyTail = nullptr;

        Thread* sleepingHead = nullptr;

        Thread* idleThread = nullptr;

        static Scheduler* instance;
    };
}


#endif //PROJECT_SCHEDULER_H
