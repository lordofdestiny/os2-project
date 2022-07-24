//
// Created by os on 7/15/22.
//
#ifndef PROJECT_MEMORYALLOCATOR_H
#define PROJECT_MEMORYALLOCATOR_H

#define ALLOCATOR MemoryAllocator::getInstance()

#include "../../lib/hw.h"
namespace kernel {
    class MemoryAllocator {
    private:
        MemoryAllocator();
        struct FreeBlock{
            FreeBlock *prev, *next;
            size_t size;

            char* end() {
                return (char*)this + size * MEM_BLOCK_SIZE;
            }
        };
        FreeBlock* head;
    public:
        MemoryAllocator(MemoryAllocator const&)=delete;
        MemoryAllocator& operator=(MemoryAllocator const&)=delete;
        static MemoryAllocator& getInstance();
        static size_t byteSizeToBlockCount(size_t blocks);
        void* allocateBlocks(size_t count);
        void* allocateBytes(size_t count);
        int deallocateBlocks(void* ptr);
    };
}

#endif //PROJECT_MEMORYALLOCATOR_H
