#ifndef PROJECT_BLOCKALLOCATOR_H
#define PROJECT_BLOCKALLOCATOR_H

#define BLOCK_MINIMUM_BITSIZE 5
#define BLOCK_MAXIMUM_BITSIZE 27
#define BLOCK_LISTS_SIZE (BLOCK_MAXIMUM_BITSIZE+1)

#include "../../../lib/hw.h"
#include "./MemoryManager.h"

namespace kernel::memory
{
    class BuddyAllocator
    {
    public:
        BuddyAllocator(BuddyAllocator const&) = delete;
        BuddyAllocator& operator=(BuddyAllocator const&) = delete;
    private:
        struct FreeBlock
        {
            FreeBlock* prev;
            FreeBlock* next;
            uint64 bits;
            static uint64 sizeToBits(uint64 size);
            void setSize(uint64 bits);
            FreeBlock* getBuddy() const;
        };
        static_assert(sizeof(FreeBlock) <= (1 << BLOCK_MINIMUM_BITSIZE));

        BuddyAllocator(MemorySection const& section);

        FreeBlock* removeBlock(FreeBlock* block);
        void insertBlock(FreeBlock* block);
        bool trySplitInto(uint64 bits);
        bool isBuddyFree(FreeBlock* block);
        void recursiveJoinBuddies(FreeBlock* block);
    public:
        static BuddyAllocator& getInstance();
        void* allocate(uint64 size);
        void deallocate(void* addr, uint64 size);

        FreeBlock* list(int i)
        {
            return blockLists[i];
        }
    private:
        FreeBlock* blockLists[BLOCK_LISTS_SIZE];
    };
}

#endif