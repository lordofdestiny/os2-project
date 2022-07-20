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
        // Disable console
        SYS_REGISTER_CLEAR_BITS(sie,BitMasks::sie::SEIE);
        SYS_REGISTER_SET_BITS(sstatus, sstatus::SIE);
    }

    void Kernel::disableInterrupts() {
        using namespace kernel::BitMasks;
        SYS_REGISTER_CLEAR_BITS(sstatus, sstatus::SIE);
    }

    void Kernel::setTrapHandler() {
        using namespace kernel::BitMasks;
        WRITE_TO_SYS_REGISTER(stvec, &TrapHandlers::supervisorTrap);
    }

    void Kernel::waitForUserThreads() {
        while (Scheduler::getInstance().hasUserThreads()) {
            thread_dispatch();
        }
    }
} // kernel