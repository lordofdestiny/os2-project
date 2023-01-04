#include "../../../h/kernel/Memory/BuddyAllocator.h"
#include "../../../h/kernel/Memory/MemoryManager.h"
#include "../../../h/ConsoleUtils.h"
#include "../../../h/kernel/Utils/Utils.h"

namespace kernel::memory
{
    inline auto BuddyAllocator::getBuddy(FreeBlock* block) const
        ->FreeBlock*
    {
        return (FreeBlock*)((uint64)block ^ sizeOfLevel(block->level));
    }

    BuddyAllocator::BuddyAllocator()
    {
        start_address = nullptr;
        end_address = nullptr;
        allocationIndex = nullptr;
        for (int i = 0; i < num_of_levels; i++)
        {
            freeLists[i] = nullptr;
        }
    }

    void BuddyAllocator::initialize(void* space, int block_num)
    {
        if (initialized) return;

        start_address = space;
        end_address = ((char*)space + block_num * PAGE_SIZE);
        num_of_levels = kernel::utils::log2(block_num) + 1;
        freeLists[0] = (FreeBlock*)space;
        freeLists[0]->prev = nullptr;
        freeLists[0]->next = nullptr;
        freeLists[0]->level = 0;

        // Allocate 2^num_of_levels ( -1 ) bits
        // for the allocationIndex
        auto order = num_of_levels - PAGE_ORDER - 3;
        if (order < 0) order = 0;
        const int level = num_of_levels - order - 1;

        for (int clevel = 0; clevel < level; )
        {
            auto block1 = removeBlock(freeLists[clevel]);

            clevel += 1;
            block1->level = clevel;
            auto block2 = getBuddy(block1);
            block2->level = clevel;

            insertBlock(block2);
            insertBlock(block1);
        }
        allocationIndex = (char*)removeBlock(freeLists[level]);
        /*Initialize index*/
        const auto indexSize = 1 << (num_of_levels - 3);
        for (int i = 0; i < indexSize; i++)
        {
            allocationIndex[i] = 0;
        }
        for (int lvl = 1; lvl < num_of_levels; lvl++)
        {
            setAllocated((1 << lvl) - 1);
        }

        initialized = true;
    }

    BuddyAllocator& BuddyAllocator::getInstance()
    {
        static BuddyAllocator instance{};
        return instance;
    }

    // O(1)
    auto BuddyAllocator::removeBlock(FreeBlock* block)
        ->FreeBlock*
    {
        const auto level = block->level;
        if (block == nullptr || freeLists[level] == nullptr)
        {
            return nullptr;
        }

        if (freeLists[level] == block)
        {
            freeLists[level] = block->next;
        }

        if (block->next != nullptr)
        {
            block->next->prev = block->prev;
        }

        if (block->prev != nullptr)
        {
            block->prev->next = block->next;
        }
        return block;
    }

    // O(1)
    void BuddyAllocator::insertBlock(FreeBlock* block)
    {
        auto head = freeLists[block->level];
        if (head != nullptr)
        {
            head->prev = block;
        }
        block->next = head;
        block->prev = nullptr;
        freeLists[block->level] = block;
    }

    // Try to split until 2^size block is found
    // O(1) - never makes more than 12 iterations
    bool BuddyAllocator::trySplitInto(int level)
    {
        // Required block already exists, no need to split
        if (freeLists[level] != nullptr) return true;

        int clevel = level;
        while (clevel >= 0 && freeLists[clevel] == nullptr)
        {
            clevel--;
        }

        if (clevel < 0) return false;

        while (clevel < level)
        {
            const auto block = freeLists[clevel];
            setAllocated(index(block));
            auto block1 = removeBlock(block);

            clevel += 1;
            block1->level = clevel;
            auto block2 = getBuddy(block1);
            block2->level = clevel;

            setFree(index(block2));
            insertBlock(block2);
            setFree(index(block1));
            insertBlock(block1);
        }

        return true;
    }

    // O(1)
    bool  BuddyAllocator::isBuddyFree(FreeBlock* block) const
    {
        const auto bIndex = index(block);
        return isFree(bIndex & 1 ? bIndex + 1 : bIndex - 1);
    }

    // O(1) - at most does 12 joins
    void BuddyAllocator::recursiveJoinBuddies(FreeBlock* block)
    {
        auto current = block;
        do
        {
            const auto buddy = getBuddy(current);
            setAllocated(index(buddy));
            removeBlock(buddy);

            current = current < buddy
                ? current
                : buddy;
            current->level--;
        } while (isBuddyFree(current));

        setFree(index(current));
        insertBlock(current);
    }

    // O(1)
    void* BuddyAllocator::allocate(
        int order,
        MemoryErrorManager& errmng)
    {
        auto escope = errmng
            .getScope(
                ErrorOrigin::BUDDY,
                Operation::ALLOCATE);
        const auto level = num_of_levels - order - 1;

        if (level < 0 || level > num_of_levels)
        {
            escope.setError(BuddyError::INVALID_PAGES_ORDER);
            return nullptr;
        }

        if (!trySplitInto(level))
        {
            escope.setError(BuddyError::ALLOCATION_FAILED);
            return nullptr;
        }

        const auto block = freeLists[level];
        setAllocated(index(block));
        return removeBlock(block);
    }

    // O(1)
    void BuddyAllocator::deallocate(
        void* addr, int order,
        MemoryErrorManager& errmng)
    {
        auto escope = errmng
            .getScope(
                ErrorOrigin::BUDDY,
                Operation::DEALLOCATE);

        if (addr == nullptr)
        {
            escope.setError(BuddyError::DEALLOCATE_NULLPTR);
            return;
        }

        if (start_address > addr || addr >= end_address)
        {
            escope.setError(BuddyError::INVALID_DEALLOCATION_ADDRESS);
            return;
        }

        const auto level = num_of_levels - order - 1;

        if (level < 0 || level > num_of_levels)
        {
            escope.setError(BuddyError::INVALID_PAGES_ORDER);
            return;
        }

        /* Refactor this*/
        auto block = (FreeBlock*)addr;
        block->level = level;
        if (isBuddyFree(block))
        {
            recursiveJoinBuddies(block);
        }
        else
        {
            setFree(index(block));
            insertBlock(block);
        }
    }

    void BuddyAllocator::printBlockTable() const
    {
        for (int i = 0; i < num_of_levels; i++)
        {
            printInt(i);
            printString(". ");
            printAddress(freeLists[i]);
            printString(" || ");
            auto p = freeLists[i];
            while (p != nullptr)
            {
                printAddress(p);
                printString(", ");
                p = p->next;
            }
            printString("\n");
        }
    }

}
