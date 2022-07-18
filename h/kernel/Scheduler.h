//
// Created by os on 7/16/22.
//

#ifndef PROJECT_SCHEDULER_H
#define PROJECT_SCHEDULER_H

#include "../../h/kernel/TCB.h"

namespace kernel {
    class Scheduler {
    public:
        Scheduler(Scheduler const &) = delete;
        Scheduler &operator=(Scheduler const &) = delete;
//        ~Scheduler();

        static Scheduler &getInstance();

        TCB *get();
        void put(TCB *thread);
        bool hasUserThreads();
    private:
        Scheduler() = default;
        TCB *getIdleThread();

        TCB* readyHead = nullptr;
        TCB* readyTail = nullptr;
//        TCB* sleepingHead = nullptr;
        TCB* idleThread = nullptr;
        size_t userThreadCount = 0;
    };
}


#endif //PROJECT_SCHEDULER_H
