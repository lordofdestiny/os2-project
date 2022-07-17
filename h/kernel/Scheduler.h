//
// Created by os on 7/16/22.
//

#ifndef PROJECT_SCHEDULER_H
#define PROJECT_SCHEDULER_H
#include "../../h/kernel/TCB.h"

namespace kernel {
    class Scheduler {
    public:
        Scheduler(Scheduler const&)=delete;
        Scheduler& operator=(Scheduler const&)=delete;

        static Scheduler& getInstance();

        TCB* get();
        void put(TCB* thread);
    private:
        ~Scheduler();

        Scheduler()=default;

        TCB* readyHead = nullptr;
        TCB* readyTail = nullptr;
//        TCB* sleepingHead = nullptr;
        TCB* idleThread = nullptr;
        TCB* getIdleThread();
    };
}


#endif //PROJECT_SCHEDULER_H
