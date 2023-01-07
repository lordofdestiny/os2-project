//
// Created by os on 7/15/22.
//
#ifndef PROJECT_MEMORYALLOCATOR_H
#define PROJECT_MEMORYALLOCATOR_H

#define ALLOCATOR memory::HeapAllocator::getInstance()

#include "../../../lib/hw.h"
namespace kernel::memory
{
    class HeapAllocator
    {
    private:
        HeapAllocator();
        struct FreeBlock
        {
            FreeBlock* prev, * next;
            size_t size;

            char* end()
            {
                return (char*)this + size * MEM_BLOCK_SIZE;
            }
        };
        FreeBlock* head;
    public:
        HeapAllocator(HeapAllocator const&) = delete;
        HeapAllocator& operator=(HeapAllocator const&) = delete;
        static HeapAllocator& getInstance();
        static size_t byteSizeToBlockCount(size_t bytes);
        void* allocateBlocks(size_t count);
        void* allocateBytes(size_t count);
        int deallocateBlocks(void* ptr);
    };
}

#endif //PROJECT_MEMORYALLOCATOR_H
