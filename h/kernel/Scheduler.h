//
// Created by os on 7/16/22.
//

#ifndef PROJECT_SCHEDULER_H
#define PROJECT_SCHEDULER_H

#include "../../h/kernel/Thread.h"

namespace kernel {
    class Scheduler final{
    public:
        Scheduler(Scheduler const &) = delete;
        Scheduler &operator=(Scheduler const &) = delete;

        static Scheduler &getInstance();

        void put(Thread *thread);
        Thread *get();

        void entrance(Thread* thread, uint64 ticks);
        void tick();
    private:
        void awaken();
        Scheduler() = default;
        Thread *getIdleThread();
    private:
        Thread* readyHead = nullptr;
        Thread* readyTail = nullptr;

        Thread* sleepingHead = nullptr;

        Thread* idleThread = nullptr;
    };
}


#endif //PROJECT_SCHEDULER_H
