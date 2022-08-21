//
// Created by os on 7/16/22.
//
#include "../../h/kernel/SystemCalls.h"
#include "../../h/kernel/TrapHandlers.h"
#include "../../h/kernel/RegisterUtils.h"
#include "../../h/kernel/Thread.h"
#include "../../h/kernel/Scheduler.h"
#include "../../h/kernel/Console.h"

namespace kernel {
    namespace TrapHandlers {
        static Handler errorHandler = nullptr;

        void setErrorHandler(Handler handler) {
            errorHandler = handler;
        }

        void instructionErrorHandler() {
            Thread::getRunning()->skipInstruction();
            if (errorHandler != nullptr) {
                errorHandler();
            }
        }

        void timerInterruptHandler() {
            Thread::getRunning()->tick();
            SCHEDULER.tick();
            SREGISTER_CLEAR_BITS(sip, BitMasks::sip::SSIP);
        }

        void hardwareInterruptHandler() {
            auto irqSrc = plic_claim();
            if (irqSrc == CONSOLE_IRQ) {
                CONSOLE.handle();
            }
            plic_complete(irqSrc);
        }

        void systemCallHandler() {
            auto type = ACCEPT(CallType, 0);

            auto handle = SYSTEMCALLS.getHandler(type);
            if (handle != nullptr) handle();

            NEXT_INSTRUCTION();
        }

        void supervisorTrapHandle() {
            using TrapType = TrapHandlers::TrapType;

            TrapType trapCause;
            SREGISTER_READ(scause, trapCause);

            switch (trapCause) {
            case TrapType::TimerTrap:
                return timerInterruptHandler();
            case TrapType::ExternalHardwareTrap:
                return hardwareInterruptHandler();
            case TrapType::UserEnvironmentCall:
            case TrapType::SystemEnvironmentCall:
                return systemCallHandler();
            case TrapType::IllegalInstruction:
            case TrapType::IllegalReadAddress:
            case TrapType::IllegalWriteAddress:
                return instructionErrorHandler();
            default:
                break;
            }
        }
    }
}



