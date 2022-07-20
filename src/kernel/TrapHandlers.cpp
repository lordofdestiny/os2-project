//
// Created by os on 7/16/22.
//
#include "../../h/kernel/SystemCalls.h"
#include "../../h/kernel/TrapHandlers.h"
#include "../../h/kernel/RegisterUtils.h"
#include "../../h/kernel/Thread.h"
#include "../../h/kernel/ConsoleUtils.h"

namespace kernel {
    namespace TrapHandlers {
        void incrementPC() {
            auto runningThread = Thread::getRunning();
            auto pc = (uint64) runningThread->getPC();
            runningThread->setPC(pc + 4);
        }

        void instructionErrorHandle() {
            incrementPC();
            uint64 temp;
            asm volatile("csrr %0, scause":"=r"(temp));
            printReg("scause", temp);
            asm volatile("csrr  %0, sepc":"=r"(temp));
            printReg("sepc", temp);
            asm volatile("csrr %0, stval":"=r"(temp));
            printReg("stval", temp);
        }

        void timerHandler() {
            Thread::tick();
            SYS_REGISTER_CLEAR_BITS(sip,BitMasks::sip::SSIP);
        }

        void systemCallHandle() {
            using namespace SystemCalls;
            auto runningThread = Thread::getRunning();
            auto type = (CallType) runningThread->getConetxt().getRegisters().a0;

            incrementPC();

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
                    break;
                case CallType::GetChar:
                    break;
                case CallType::PutChar:
                    break;
            }
        }

        void supervisorTrapHandle() {
            using TrapType = TrapHandlers::TrapType;

            TrapType trapCause;
            READ_FROM_SYS_REGISTER(scause, trapCause);

            switch (trapCause) {
                case TrapType::TimerTrap:
                    return timerHandler();
                case TrapType::ExternalHardwareTrap:
                    break;
                case TrapType::UserEnvironmentCall:
                case TrapType::SystemEnvironmentCall:
                    return systemCallHandle();
                case TrapType::IllegalInstruction:
                case TrapType::IllegalReadAddress:
                case TrapType::IllegalWriteAddress:
                    return instructionErrorHandle();
                default:
                    break;
            }
        }
    }
}



