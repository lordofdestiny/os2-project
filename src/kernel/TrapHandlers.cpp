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
            if(errorHandler != nullptr) {
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
            if(irqSrc == CONSOLE_IRQ) {
                CONSOLE.handle();
            }
            plic_complete(irqSrc);
        }

        void systemCallHandler() {
            using namespace SystemCalls;
            auto type = ACCEPT(CallType, 0);

            NEXT_INSTRUCTION();

            switch (type) {
                case CallType::MemoryAllocate:
                    return mem_alloc();
                case CallType::MemoryFree:
                    return mem_free();
                case CallType::ThreadCreate:
                    return thread_create();
                case CallType::ThreadExit:
                    return thread_exit();
                case CallType::ThreadDispatch:
                    return Thread::dispatch();
                case CallType::SemaphoreOpen:
                    return sem_open();
                case CallType::SemaphoreClose:
                    return sem_close();
                case CallType::SemaphoreWait:
                    return sem_wait();
                case CallType::SemaphoreSignal:
                    return sem_signal();
                case CallType::TimeSleep:
                    return time_sleep();
                case CallType::GetChar:
                    return getc();
                case CallType::PutChar:
                    return putc();
                case CallType::EnterUserMode:
                    return enter_user_mode();
            }
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



