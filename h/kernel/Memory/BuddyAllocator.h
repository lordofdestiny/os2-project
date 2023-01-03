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
        void initialize(void* space, int block_num);

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

        inline size_t totalSize() const
        {
            return 1 << (num_of_levels - 1 + PAGE_ORDER);
        }
        inline size_t sizeOfLevel(int level) const
        {
            return totalSize() >> level;
        }
        inline size_t indexInLevelOf(void* block, int level) const
        {
            return ((uint64)block - (uint64)start_address) / sizeOfLevel(level);
        }
        inline size_t index(FreeBlock* block) const
        {
            return (1 << block->level) + indexInLevelOf(block, block->level) - 1;
        }

        void setIsAllocated(size_t index) const
        {
            isAllocated[index / 8] |= 1 << (index % 8);
        }

        void setIsNotAllocated(size_t index) const
        {
            isAllocated[index / 8] &= ~(1 << (index % 8));
        }

        bool isNotAllocatedBlock(size_t index) const
        {
            return (isAllocated[index / 8] & (1 << (index % 8))) == 0;
        }

        /* Points to page(s) of this buffer that are used to track
         * whether the buddies are free or not. Each pair of buddies requires a single bit */
        char* isAllocated;
    };
}

#endif