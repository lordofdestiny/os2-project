//
// Created by os on 7/19/22.
//

#include "../../h/kernel/Kernel.h"
#include "../../h/kernel/Utils/RegisterUtils.h"
#include "../../h/kernel/SystemCalls/SystemCalls.h"
#include "../../h/kernel/Console/Console.h"

#define BLOCK_ON_ERROR

#ifdef BLOCK_ON_ERROR
const static bool block = true;
#else
const static bool block = false;
#endif

namespace kernel {
    alignas(uint16) uint8 Kernel::kernelStack[Kernel::stackSize];
    uint8* Kernel::kernelStackTopAddress = ((uint8*)&Kernel::kernelStack + stackSize);
    sem_t Kernel::userMainDone;
    thread_t Kernel::userMainThread;


    void Kernel::initialize() {
        setTrapHandler(block);
        SYSTEMCALLS.initialize();
        Thread::initialize();
        CONSOLE.initialize();
        enableInterrupts();
        sem_open(&Kernel::userMainDone, 0);
    }

    void Kernel::execute(TMain userMain) {
        struct Shadow {
            TMain main;
            sem_t sem;
        } argument{ userMain, userMainDone };

        thread_create(
            &userMainThread,
            [](void* args) {
                auto sh = (Shadow*)args;
                enter_user_mode();
                sh->main();
                sem_signal(sh->sem);
            }, &argument);
        sem_wait(userMainDone);
    }

    void Kernel::finalize() {
        sem_close(userMainDone);
        waitForUserThreads();
        Thread::setMainFinished();
        CONSOLE.join();
    }

    void Kernel::enableInterrupts() {
        // Enable external interrupts
        SREGISTER_SET_BITS(sstatus, BitMasks::sstatus::SIE);
    }

    void Kernel::setTrapHandler(bool blockOnError) {
        TrapHandlers::Handler errorHandler;
        if (blockOnError) {
            SREGISTER_READ(stvec, errorHandler);
            TrapHandlers::setErrorHandler(errorHandler);
        }
        SREGISTER_WRITE(stvec, &TrapHandlers::supervisorTrap);
    }

    void Kernel::waitForUserThreads() {
        while (Thread::getCount(Thread::Mode::USER) > 1) {
            thread_dispatch();
        }
    }
} // kernel