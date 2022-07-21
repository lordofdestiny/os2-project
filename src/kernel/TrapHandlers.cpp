//
// Created by os on 7/16/22.
//
#include "../../h/kernel/SystemCalls.h"
#include "../../h/kernel/TrapHandlers.h"
#include "../../h/kernel/RegisterUtils.h"
#include "../../h/kernel/Thread.h"
#include "../../h/kernel/ConsoleUtils.h"
#include "../../h/kernel/Scheduler.h"

namespace kernel {
    namespace TrapHandlers {
        void defaultErrorHandler() {
            uint64 temp;
            SREGISTER_READ(scause, temp);
            printReg("scause", temp);
            SREGISTER_READ(sepc, temp);
            printReg("sepc", temp);
            SREGISTER_READ(stval, temp);
            printReg("stval", temp);
        }

        static Handler errorHandler = defaultErrorHandler;

        void setErrorHandler(Handler handler) {
            errorHandler = handler;
        }

        void instructionErrorHandler() {
            errorHandler();
            if(errorHandler == &defaultErrorHandler){
                Thread::getRunning()->skipInstruction();
            }
        }

        void timerHandler() {
            Thread::getRunning()->tick();
            Scheduler::getInstance().tick();
            SREGISTER_CLEAR_BITS(sip, BitMasks::sip::SSIP);
        }

        void systemCallHandler() {
            using namespace SystemCalls;
            auto runningThread = Thread::getRunning();
            auto type = (CallType) runningThread->getContext().getRegisters().a0;

            runningThread->skipInstruction();

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
            SREGISTER_READ(scause, trapCause);

            switch (trapCause) {
                case TrapType::TimerTrap:
                    return timerHandler();
                case TrapType::ExternalHardwareTrap:
                    break;
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



