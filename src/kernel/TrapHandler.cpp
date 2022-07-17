//
// Created by os on 7/16/22.
//
#include "../../h/kernel/SystemCalls.h"
#include "../../h/kernel/TrapHandler.h"
#include "../../h/kernel/RegisterUtils.h"

extern "C" void incrementSEPC();

    void kernel::TrapHandler::instructionErrorHandle() {
        incrementSEPC();
    }

    void kernel::TrapHandler::supervisorTrapHandle() {
        using TrapType = TrapHandler::TrapType;

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
                instructionErrorHandle();
        }
    }
