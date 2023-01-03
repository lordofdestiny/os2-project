#include "../../../h/kernel/Memory/BuddyAllocator.h"
#include "../../../h/kernel/Memory/MemoryManager.h"
#include "../../../h/ConsoleUtils.h"
#include "../../../h/kernel/Utils/Utils.h"

namespace kernel::memory
{
    inline void BuddyAllocator::FreeBlock::setLevel(int level)
    {
        this->level = level;
    }

    inline auto BuddyAllocator::getBuddy(FreeBlock* block) const
        ->FreeBlock*
    {
        return (FreeBlock*)((uint64)block ^ sizeOfLevel(block->level));
    }

    BuddyAllocator::BuddyAllocator()
    {
        start_address = nullptr;
        end_address = nullptr;
        for (int i = 0; i < num_of_levels; i++)
        {
            freeLists[i] = nullptr;
        }
    }

    void BuddyAllocator::initialize(void* space, int block_num)
    {
        auto& instance = getInstance();
        if (!instance.initialized)
        {
            instance.start_address = space;
            instance.end_address = ((char*)space + block_num * PAGE_SIZE);
            instance.num_of_levels = kernel::utils::log2(block_num) + 1;
            instance.freeLists[0] = (FreeBlock*)space;
            instance.freeLists[0]->prev = nullptr;
            instance.freeLists[0]->next = nullptr;
            instance.freeLists[0]->level = 0;
            instance.initialized = true;
        }
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
        if (block == nullptr
            || freeLists[block->level] == nullptr)
        {
            return nullptr;
        }

        if (freeLists[block->level] == block)
        {
            freeLists[block->level] = block->next;
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
    void BuddyAllocator::insertBlock(
        FreeBlock* block)
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
        while (clevel >= 0
            && freeLists[clevel] == nullptr)
        {
            clevel--;
        }

        if (clevel < 0) return false;

        while (clevel < level)
        {
            auto block1 = removeBlock(freeLists[clevel]);
            clevel += 1;
            block1->setLevel(clevel);
            auto block2 = getBuddy(block1);
            block2->setLevel(clevel);

            insertBlock(block2);
            insertBlock(block1);
        }

        return true;
    }

    // O(n) --> must go to O(1), needs 2^12 booleans
    bool  BuddyAllocator::isBuddyFree(FreeBlock* block) const
    {
        auto buddy = getBuddy(block);

        auto curr = freeLists[block->level];
        while (curr != nullptr)
        {
            if ((char*)curr == (char*)buddy) return true;
            curr = curr->next;
        }
        return false;
    }

    // O(1) - at most does 12 joins
    void BuddyAllocator::recursiveJoinBuddies(FreeBlock* block)
    {
        /* Assuems there exists a free buddy at block's level*/
        auto current = block;
        do
        {
            const auto buddy = getBuddy(current);
            removeBlock(buddy);

            current = current < buddy
                ? current
                : buddy;
            current->level--;

        } while (isBuddyFree(current));

        insertBlock(current);
    }

    // O(1)
    void* BuddyAllocator::allocate(
        int order,
        MemoryErrorManager& errmng)
    {
        auto escope = errmng.getScope(ErrorOrigin::BUDDY, Operation::ALLOCATE);
        const auto blockLevel = num_of_levels - order - 1;

        if (blockLevel < 0 || blockLevel > num_of_levels)
        {
            escope.setError(BuddyError::INVALID_PAGES_ORDER);
            return nullptr;
        }

        if (!trySplitInto(blockLevel))
        {
            escope.setError(BuddyError::ALLOCATION_FAILED);
            return nullptr;
        }

        return removeBlock(freeLists[blockLevel]);
    }

    // O(1) +? O(isBuddyFree)*O(recursiveJoinBuddies)
    void BuddyAllocator::deallocate(
        void* addr, int order,
        MemoryErrorManager& errmng)
    {
        auto escope = errmng.getScope(ErrorOrigin::BUDDY, Operation::DEALLOCATE);

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

        const auto blockLevel = num_of_levels - order - 1;

        if (blockLevel < 0 || blockLevel > num_of_levels)
        {
            escope.setError(BuddyError::INVALID_PAGES_ORDER);
            return;
        }

        auto block = (FreeBlock*)addr;
        block->setLevel(blockLevel);
        if (isBuddyFree(block))
        {
            recursiveJoinBuddies(block);
        }
        else insertBlock(block);

    }

    void BuddyAllocator::printBlockTable() const
    {
        for (int i = 0; i < num_of_levels; i++)
        {
            printInt(i);
            printString(". ");
            printAddress(freeLists[i]);
            printString("\n");
        }
    }

    size_t BuddyAllocator::totalSize() const
    {
        return (1 << (num_of_levels - 1)) * PAGE_SIZE;
    }

    size_t BuddyAllocator::sizeOfLevel(int level) const
    {
        return totalSize() / (1 << level);
    }

    size_t BuddyAllocator::indexInLevelOf(void* block, int level) const
    {
        return ((uint64)block - (uint64)start_address) / sizeOfLevel(level);
    }
}
