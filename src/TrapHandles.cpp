//
// Created by os on 7/16/22.
//
#include "../h/SystemCalls.h"
#include "../h/TrapHandles.h"
#include "../h/RegisterUtils.h"

extern "C" void incrementSEPC();

extern "C" void supervisorTrapHandle() {
    using kernel::TrapType;

    TrapType trapCause;
    READ_FROM_SYS_REGISTER(scause, trapCause);

    switch(trapCause) {
        case TrapType::TimerTrap:
            break;
        case TrapType::ExternalHardwareTrap:
            break;
        case TrapType::UserSystemCall:
        case TrapType::SystemSystemCall:
            return kernel::SystemCalls::handle();
        case TrapType::IllegalInstruction:
        case TrapType::IllegalReadAddress:
        case TrapType::IllegalWriteAddress:
        default:
            kernel::instructionErrorHandle();
    }
}

namespace kernel {
    void instructionErrorHandle() {
        incrementSEPC();
    }
}