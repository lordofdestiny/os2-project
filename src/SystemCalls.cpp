//
// Created by os on 7/16/22.
//

#include "../h/SystemCalls.h"
#include "../h/MemoryAllocator.h"
#include "../h/RegisterUtils.h"
#include "../lib/hw.h"

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
                break;
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