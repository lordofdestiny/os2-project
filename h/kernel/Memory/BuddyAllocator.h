#ifndef PROJECT_BUDDY_LLOCATOR_H
#define PROJECT_BUDDY_LLOCATOR_H

#define MIN_ORDER 12
#define MAX_ORDER 24
#define BLOCK_LISTS_SIZE (MAX_ORDER-MIN_ORDER+1)
#define PAGE_SIZE (1 << MIN_ORDER)


#include "../../../lib/hw.h"
#include "./MemoryManager.h"
#include  "./MemoryErrorManager.h"

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

        BuddyAllocator();

        FreeBlock* removeBlock(FreeBlock* block);
        void insertBlock(FreeBlock* block);
        bool trySplitInto(unsigned int order);
        bool isBuddyFree(FreeBlock* block) const;
        void recursiveJoinBuddies(FreeBlock* block);
    public:
        static BuddyAllocator& getInstance();
        static void initialize(void* space, int block_num);

        void* allocate(unsigned int order,
            MemoryErrorManager& emg);
        void deallocate(void* addr, unsigned int order,
            MemoryErrorManager& emg);

        void printBlockTable() const;
    private:
        bool initialized = false;
        void* start_address = nullptr;
        void* end_address = nullptr;

        FreeBlock* blockLists[BLOCK_LISTS_SIZE];

        inline FreeBlock*& operator[](size_t i);
        inline FreeBlock* const& operator[](size_t i) const;
    };
}

#endif