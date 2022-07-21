//
// Created by os on 7/15/22.
//
#ifndef PROJECT_MEMORYALLOCATOR_H
#define PROJECT_MEMORYALLOCATOR_H

#include "../../lib/hw.h"
namespace kernel {
    class MemoryAllocator final {
    private:
        MemoryAllocator();
        struct FreeBlock{
            FreeBlock *prev = nullptr;
            FreeBlock *next = nullptr;
            size_t size= 0;

            FreeBlock(FreeBlock const& base)=default;

            FreeBlock(size_t size) : size(size) { }

            static void* operator new(size_t, void* ptr) noexcept {
                return ptr;
            };

            char* end() {
                return (char*)this + size * MEM_BLOCK_SIZE;
            }
        };
        FreeBlock* head = nullptr;
    public:
        MemoryAllocator(MemoryAllocator const&)=delete;
        MemoryAllocator& operator=(MemoryAllocator const&)=delete;
        static MemoryAllocator& getInstance();
        static size_t byteSizeToBlockCount(size_t blocks);
        void* allocateBlocks(size_t count);
        void* allocateBytes(size_t byteCount);
        int deallocateBlocks(void* ptr);
    };
}

#endif //PROJECT_MEMORYALLOCATOR_H
