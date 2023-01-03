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
        isAllocated = nullptr;
        for (int i = 0; i < num_of_levels; i++)
        {
            freeLists[i] = nullptr;
        }
    }

    void BuddyAllocator::initialize(void* space, int block_num)
    {
        if (!initialized)
        {
            start_address = space;
            end_address = ((char*)space + block_num * PAGE_SIZE);
            num_of_levels = kernel::utils::log2(block_num) + 1;
            freeLists[0] = (FreeBlock*)space;
            freeLists[0]->prev = nullptr;
            freeLists[0]->next = nullptr;
            freeLists[0]->level = 0;

            // Allocate 2^num_of_levels ( -1 ) bytes
            auto order = num_of_levels - 3 - PAGE_ORDER;
            if (order < 0) order = 0;
            const int level = num_of_levels - order - 1;

            for (int clevel = 0; clevel < level; )
            {
                auto block1 = removeBlock(freeLists[clevel]);

                clevel += 1;
                block1->setLevel(clevel);
                auto block2 = getBuddy(block1);
                block2->setLevel(clevel);

                insertBlock(block2);
                insertBlock(block1);
            }
            isAllocated = (char*)removeBlock(freeLists[level]);
            /*Initialize isAllocated*/
            isAllocated[0] = 0;
            for (int i = 0; i < 1 << (num_of_levels - 3); i++)
            {
                isAllocated[i] = 0;
            }
            for (int lvl = 1; lvl < num_of_levels; lvl++)
            {
                setIsAllocated((1 << lvl) - 1);
            }

            initialized = true;
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
        while (clevel >= 0 && freeLists[clevel] == nullptr)
        {
            clevel--;
        }

        if (clevel < 0) return false;

        while (clevel < level)
        {
            // Set as allocated
            const auto block = freeLists[clevel];
            setIsAllocated(index(block));
            auto block1 = removeBlock(block);

            clevel += 1;
            block1->setLevel(clevel);
            auto block2 = getBuddy(block1);
            block2->setLevel(clevel);

            setIsNotAllocated(index(block2));
            insertBlock(block2);
            setIsNotAllocated(index(block1));
            insertBlock(block1);
        }

        return true;
    }

    // O(1)
    bool  BuddyAllocator::isBuddyFree(FreeBlock* block) const
    {
        return isNotAllocatedBlock(
            index(block) % 2 == 0
            ? index(block) - 1
            : index(block) + 1);
    }

    // O(1) - at most does 12 joins
    void BuddyAllocator::recursiveJoinBuddies(FreeBlock* block)
    {
        /* Assuems there exists a free buddy at block's level*/
        auto current = block;
        do
        {
            // set allocated buddy of current
            const auto buddy = getBuddy(current);
            setIsAllocated(index(buddy));
            removeBlock(buddy);

            current = current < buddy
                ? current
                : buddy;
            current->level--;

        } while (isBuddyFree(current));

        // Set as free
        setIsNotAllocated(index(current));
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

        // set allocated buddy of current
        const auto block = freeLists[blockLevel];
        setIsAllocated(index(block));
        return removeBlock(block);
    }

    // O(1)
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
        else
        {
            setIsNotAllocated(index(block));
            insertBlock(block); // set block as free
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
