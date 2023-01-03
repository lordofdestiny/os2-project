#ifndef PROJECT_BUDDY_LLOCATOR_H
#define PROJECT_BUDDY_LLOCATOR_H

#define PAGE_ORDER 12
#define MAX_LEVEL 32
#define PAGE_SIZE (1 << PAGE_ORDER)


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
            int level;
            void setLevel(int level);
        };
        static_assert(sizeof(FreeBlock) <= (1 << PAGE_ORDER));

        BuddyAllocator();

        FreeBlock* getBuddy(FreeBlock* block) const;
        FreeBlock* removeBlock(FreeBlock* block);
        void insertBlock(FreeBlock* block);
        bool trySplitInto(int level);
        bool isBuddyFree(FreeBlock* block) const;
        void recursiveJoinBuddies(FreeBlock* block);
    public:
        static BuddyAllocator& getInstance();
        static void initialize(void* space, int block_num);

        void* allocate(int order,
            MemoryErrorManager& emg);
        void deallocate(void* addr, int order,
            MemoryErrorManager& emg);

        void printBlockTable() const;
    private:
        bool initialized = false;
        void* start_address = nullptr;
        void* end_address = nullptr;
        int num_of_levels = 0;

        FreeBlock* freeLists[MAX_LEVEL];
        size_t totalSize() const;
        size_t sizeOfLevel(int level) const;
        size_t indexInLevelOf(void* block, int level) const;
        /*
            Use to try and optimze checking buddy's status;
            One bit per buddy pair is enough to keep track of
            their status
        */
    };
}

#endif