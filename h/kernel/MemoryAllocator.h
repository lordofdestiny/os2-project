//
// Created by os on 7/15/22.
//
#ifndef PROJECT_MEMORYALLOCATOR_H
#define PROJECT_MEMORYALLOCATOR_H

#include "../../lib/hw.h"
namespace kernel {
    class MemoryAllocator {
    private:
        MemoryAllocator();
        struct FreeMemoryBlock{
            FreeMemoryBlock *prev, *next;
            size_t size;
        };
        FreeMemoryBlock* freeMemoryHead;
    public:
        MemoryAllocator(MemoryAllocator const&)=delete;
        MemoryAllocator& operator=(MemoryAllocator const&)=delete;
        static MemoryAllocator& getInstance();
        static size_t byteSizeToBlockCount(size_t blocks);
        void* allocateBlocks(size_t blockCount);
        void* allocateBytes(size_t byteCount);
        int deallocateBlocks(void* address);
    };
}

#endif //PROJECT_MEMORYALLOCATOR_H
