//
// Created by os on 7/16/22.
//

#ifndef PROJECT_SCHEDULER_H
#define PROJECT_SCHEDULER_H

#include "../../h/kernel/Thread.h"

namespace kernel {
    class Scheduler {
    public:
        Scheduler(Scheduler const &) = delete;
        Scheduler &operator=(Scheduler const &) = delete;

        static Scheduler &getInstance();

        void put(Thread *thread);
        Thread *get();

        void putToSleep(Thread* thread, uint64 ticks);
        void tick();

        bool hasUserThreads() const;
    private:
        void wake();
    private:
        Scheduler() = default;
        Thread *getIdleThread();

        Thread* readyHead = nullptr;
        Thread* readyTail = nullptr;

        Thread* sleepingHead = nullptr;

        Thread* idleThread = nullptr;
        size_t userThreadCount = 0;
    };
}


#endif //PROJECT_SCHEDULER_H
