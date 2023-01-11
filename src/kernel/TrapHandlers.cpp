//
// Created by os on 7/16/22.
//
#include "../../h/kernel/SystemCalls/SystemCalls.h"
#include "../../h/kernel/TrapHandlers.h"
#include "../../h/kernel/Utils/RegisterUtils.h"
#include "../../h/kernel/Thread.h"
#include "../../h/kernel/Scheduler.h"
#include "../../h/kernel/Console/Console.h"
#include "../../h/kernel/Memory/Mapping.h"

namespace kernel
{
    namespace TrapHandlers
    {
        static Handler errorHandler = nullptr;

        void setErrorHandler(Handler handler)
        {
            errorHandler = handler;
        }

        void instructionErrorHandler()
        {
            if (errorHandler != nullptr)
            {
                errorHandler();
            }
            Thread::getRunning()->skipInstruction();
        }

        void timerInterruptHandler()
        {
            Thread::getRunning()->tick();
            SCHEDULER.tick();
            SREGISTER_CLEAR_BITS(sip, BitMasks::sip::SSIP);
        }

        void hardwareInterruptHandler()
        {
            auto irqSrc = plic_claim();
            if (irqSrc == CONSOLE_IRQ)
            {
                CONSOLE.handle();
            }
            plic_complete(irqSrc);
        }

        void systemCallHandler()
        {
            auto type = ACCEPT(SystemCallCode, 0);

            NEXT_INSTRUCTION();

            auto handle = SYSTEMCALLS.getHandler(type);
            if (handle != nullptr) handle();
        }

        void supervisorTrapHandle()
        {
            using TrapType = TrapHandlers::TrapType;

            TrapType trapCause;
            SREGISTER_READ(scause, trapCause);

            switch (trapCause)
            {
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
            case TrapType::LoadPageFault:
            case TrapType::StorePageFault:
            case TrapType::InstructionPageFault:
            {
                using namespace kernel;
                using namespace kernel::memory;
                uint64 va = 0;
                SREGISTER_READ(stval, va);

                uint64 v_satp = 0;
                SREGISTER_READ(satp, v_satp);
                auto table = (pagetable_t)(v_satp << 12);

                uint64 v_sstatus;
                SREGISTER_READ(sstatus, v_sstatus);

                auto pte = walk(table, va, 0);
                if (pte == nullptr)  return instructionErrorHandler();


                auto is_user = (int)((v_sstatus & (uint64)BitMasks::sstatus::SPP) == 0);

                uint64 pte_v = 0;
                REGISTER_WRITE(t0, pte);
                asm volatile("sfence.vma zero, zero");
                asm volatile("ld t1, 0(t0)");
                asm volatile("sfence.vma zero, zero;");
                REGISTER_READ(t1, pte_v);
                pte_v = (pte_v & ~PTE_U) | (is_user << PTE_U);
                REGISTER_WRITE(t1, pte_v);
                asm volatile("sfence.vma zero, zero");
                asm volatile("sd t1, 0(t0)");
                asm volatile("sfence.vma zero, zero;");
                break;
            }
            default:
                while (true);
                break;
            }
        }
    }
}



