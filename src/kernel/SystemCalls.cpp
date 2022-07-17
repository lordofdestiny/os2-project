//
// Created by os on 7/16/22.
//

#include "../../h/kernel/SystemCalls.h"
#include "../../h/kernel/MemoryAllocator.h"
#include "../../h/kernel/RegisterUtils.h"
#include "../../h/kernel/TCB.h"
#include "../../lib/hw.h"
#include "../../h/syscall_c.h"

extern "C" void incrementSEPC();

namespace kernel {

    void SystemCalls::mem_alloc() {
        size_t blockCount;
        asm volatile("ld %[count], 0x58(t0)":[count]"=r"(blockCount));
        void* memory = MemoryAllocator::getInstance().allocateBlocks(blockCount);
        asm volatile("sd %[address], 0x50(t0)"::[address]"r"(memory));
    }

    void SystemCalls::mem_free(){
        void* memory;
        asm volatile("ld %[count], 0x58(t0)":[count]"=r"(memory));
        int code = MemoryAllocator::getInstance().deallocateBlocks(memory);
        asm volatile("sd %0, 0x50(t0)"::"r"(code));
    }

    void SystemCalls::thread_create() {
        thread_t* handle;
        asm volatile("ld %[dest], 0x58(t0)":[dest]"=r"(handle));
        TCB::Task task;
        asm volatile("ld %[ptr], 0x60(t0)":[ptr]"=r"(task));
        void* argument;
        asm volatile("ld %[arg], 0x68(t0)":[arg]"=r"(argument));
        void* stack;
        asm volatile("ld %[stack], 0x70(t0)":[stack]"=r"(stack));

        *handle = (thread_t) new TCB(task,argument,stack);
        int returnCode;
        if(*handle) {
            // Add to sheduler
            returnCode = 0x00;
        }else {
            returnCode = -0x01;
        }
        asm volatile("sd %0, 0x50(t0)"::"r"(returnCode));
    }

    void SystemCalls::handle() {
        incrementSEPC();

        Type type;
        asm volatile("csrr t0, sscratch");
        asm volatile("ld %[count], 0x50(t0)":[count]"=r"(type));

        switch (type) {
            case Type::MemoryAllocate:
                return mem_alloc();
            case Type::MemoryFree:
                return mem_free();
            case Type::ThreadCreate:
                return thread_create();
            case Type::ThreadExit:
                break;
            case Type::ThreadDispatch:
                break;
            case Type::SemaphoreOpen:
                break;
            case Type::SemaphoreClose:
                break;
            case Type::SemaphoreWait:
                break;
            case Type::SemaphoreSignal:
                break;
            case Type::TimeSleep:
                break;
            case Type::GetChar:
                break;
            case Type::PutChar:
                break;
        }

    }
} // kernel