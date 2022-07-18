//
// Created by os on 7/16/22.
//
#include "../../h/kernel/SystemCalls.h"
#include "../../h/kernel/TrapHandler.h"
#include "../../h/kernel/RegisterUtils.h"
#include "../../h/kernel/TCB.h"
#include "../../h/kernel/ConsoleUtils.h"


void kernel::TrapHandler::instructionErrorHandle() {
    incrementPC();
    uint64 temp;
    asm volatile("csrr %0, scause":"=r"(temp));
    printReg("scause",temp);
    asm volatile("csrr  %0, sepc":"=r"(temp));
    printReg("sepc",temp);
    asm volatile("csrr %0, stval":"=r"(temp));
    printReg("stval",temp);
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
            case TrapType::UserEnvironmentCall:
            case TrapType::SystemEnvironmentCall:
                return kernel::SystemCalls::handle();
            case TrapType::IllegalInstruction:
            case TrapType::IllegalReadAddress:
            case TrapType::IllegalWriteAddress:
            default:
                instructionErrorHandle();
        }
    }

void kernel::TrapHandler::incrementPC(){
    auto runningThread = kernel::TCB::getRunningThread();
    auto pc = (uint64) runningThread->getPC();
    runningThread->setPC((uint64*)(pc + 4));
}