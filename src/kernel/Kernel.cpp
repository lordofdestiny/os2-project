//
// Created by os on 7/19/22.
//

#include "../../h/kernel/Kernel.h"
#include "../../h/kernel/RegisterUtils.h"
#include "../../h/kernel/TrapHandlers.h"
#include "../../h/syscall_c.h"

namespace kernel {
    alignas(uint16) uint8 Kernel::kernelStack[Kernel::stackSize];
    void* Kernel::kernelStackTopAddress= (void*)((uint64) &Kernel::kernelStack + stackSize);

    void Kernel::initialize() {
        setTrapHandler();
        Thread::initialize();
        enableInterrupts();
    }

    void Kernel::finalize() {
        waitForUserThreads();
        disableInterrupts();
    }

    void Kernel::enableInterrupts() {
        using namespace kernel::BitMasks;
        // Disable external hardware interrupts
        SREGISTER_CLEAR_BITS(sie, BitMasks::sie::SEIE);
        // Enable external interrupts
        SREGISTER_SET_BITS(sstatus, sstatus::SIE);
    }

    void Kernel::disableInterrupts() {
        using namespace kernel::BitMasks;
        SREGISTER_CLEAR_BITS(sstatus, sstatus::SIE);
    }

    void Kernel::setTrapHandler(bool keepErrorHandler) {
        using namespace kernel::BitMasks;
        TrapHandlers::Handler errorHandler;
        if(keepErrorHandler) {
            SREGISTER_READ(stvec, errorHandler);
            TrapHandlers::setErrorHandler(errorHandler);
        }
        SREGISTER_WRITE(stvec, &TrapHandlers::supervisorTrap);
    }

    void Kernel::waitForUserThreads() {
        while (Scheduler::getInstance().hasUserThreads()) {
            thread_dispatch();
        }
    }
} // kernel