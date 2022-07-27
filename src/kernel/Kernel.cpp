//
// Created by os on 7/19/22.
//

#include "../../h/kernel/Kernel.h"
#include "../../h/kernel/RegisterUtils.h"
#include "../../h/syscall_c.h"
#include "../../h/kernel/Console.h"

#define BLOCK_ON_ERROR

#ifdef BLOCK_ON_ERROR
const static bool block = true;
#else
const static bool block = false;
#endif

namespace kernel {
    alignas(uint16) uint8 Kernel::kernelStack[Kernel::stackSize];
    uint8* Kernel::kernelStackTopAddress = ((uint8*) &Kernel::kernelStack + stackSize);

    void Kernel::initialize() {
        setTrapHandler(block);
        Thread::initialize();
        enableInterrupts();
        CONSOLE.initialize();
        enter_user_mode();
    }

    void Kernel::finalize() {
        waitForUserThreads();
        Thread::setMainFinished();
        thread_dispatch(); // Wait for console to finish
    }

    void Kernel::enableInterrupts() {
        using namespace kernel::BitMasks;
        // Enable external interrupts
        SREGISTER_SET_BITS(sstatus, sstatus::SIE);
    }

    void Kernel::setTrapHandler(bool blockOnError) {
        TrapHandlers::Handler errorHandler;
        if(blockOnError) {
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