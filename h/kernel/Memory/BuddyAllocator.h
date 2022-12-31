#ifndef PROJECT_BUDDY_LLOCATOR_H
#define PROJECT_BUDDY_LLOCATOR_H

#define MIN_ORDER 12
#define MAX_ORDER 27
#define BLOCK_LISTS_SIZE (MAX_ORDER-MIN_ORDER+1)
#define PAGE_SIZE (1 << MIN_ORDER)


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
            unsigned int order;
            void setOrder(unsigned int order);
            FreeBlock* getBuddy() const;
        };
        static_assert(sizeof(FreeBlock) <= (1 << MIN_ORDER));

        BuddyAllocator(MemorySection const& section);

        FreeBlock* removeBlock(FreeBlock* block);
        void insertBlock(FreeBlock* block);
        bool trySplitInto(unsigned int order);
        bool isBuddyFree(FreeBlock* block);
        void recursiveJoinBuddies(FreeBlock* block);
    public:
        static BuddyAllocator& getInstance();
        void* allocate(unsigned int order);
        void deallocate(void* addr, unsigned int order);
        void printBlockTable();
    private:
        FreeBlock* blockLists[BLOCK_LISTS_SIZE];
        inline FreeBlock*& operator[](size_t i);
    };
}

#endif