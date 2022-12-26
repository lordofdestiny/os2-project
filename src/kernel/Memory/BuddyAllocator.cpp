#include "../../../h/kernel/Memory/BuddyAllocator.h"
#include "../../../h/kernel/Memory/MemoryManager.h"
#include "../../../h/ConsoleUtils.h"
#include "../../../h/kernel/Utils/Math.h"

namespace kernel::memory
{
    inline void BuddyAllocator::FreeBlock::setSize(uint64 size)
    {
        this->size = size;
    }

    inline auto BuddyAllocator::FreeBlock::getBuddy() const
        ->FreeBlock*
    {
        return (FreeBlock*)((uint64)this ^ (1 << size));
    }

    BuddyAllocator::BuddyAllocator(
        MemorySection const& section
    )
    {
        for (int i = 0; i < BLOCK_LISTS_SIZE; i++)
        {
            blockLists[i] = nullptr;
        }

        const auto max = log2(section.size());
        blockLists[max] = (FreeBlock*)section.startAddress;
        blockLists[max]->prev = nullptr;
        blockLists[max]->next = nullptr;
        blockLists[max]->setSize(max);
    }

    BuddyAllocator& BuddyAllocator::getInstance()
    {
        static auto bounds = kernelSectionBounds();
        static BuddyAllocator instance{ bounds };

        return instance;
    }

    auto BuddyAllocator::removeBlock(FreeBlock* block)
        ->FreeBlock*
    {
        if (block == nullptr
            || blockLists[block->size] == nullptr)
        {
            return nullptr;
        }

        if (blockLists[block->size] == block)
        {
            blockLists[block->size] = block->next;
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

    void BuddyAllocator::insertBlock(
        FreeBlock* block)
    {
        auto head = blockLists[block->size];
        if (head != nullptr)
        {
            head->prev = block;
        }
        block->next = head;
        block->prev = nullptr;
        blockLists[block->size] = block;
    }

    // Try to split until 2^size block is found
    bool BuddyAllocator::trySplitInto(uint64 size)
    {
        // Required block already exists, no need to split
        if (blockLists[size] != nullptr) return true;

        auto csize = size;
        while (csize < BLOCK_LISTS_SIZE
            && blockLists[csize] == nullptr)
        {
            csize++;
        }

        if (csize == BLOCK_LISTS_SIZE) return false;

        while (csize > size)
        {
            auto block1 = removeBlock(blockLists[csize]);
            csize -= 1;
            block1->setSize(csize);
            auto block2 = block1->getBuddy();
            block2->setSize(csize);

            insertBlock(block2);
            insertBlock(block1);
        }

        return true;
    }

    bool  BuddyAllocator::isBuddyFree(FreeBlock* block)
    {
        void* buddy = block->getBuddy();

        auto curr = blockLists[block->size];
        while (curr != nullptr)
        {
            if ((char*)curr == (char*)buddy) return true;
            curr = curr->next;
        }
        return false;
    }

    void BuddyAllocator::recursiveJoinBuddies(FreeBlock* block)
    {
        /* Assuems there exists a free buddy at block's level*/
        auto current = block;
        do
        {
            const auto buddy = current->getBuddy();
            removeBlock(buddy);

            current = (uint64)current < (uint64)buddy
                ? current
                : buddy;
            current->size++;
        } while (isBuddyFree(current));
        insertBlock(current);
    }

    void* BuddyAllocator::allocate(uint64 size)
    {
        if (size < BLOCK_MINIMUM_BITSIZE
            || size > BLOCK_MAXIMUM_BITSIZE)
        {
            return nullptr;
        }

        if (!trySplitInto(size)) return nullptr;

        return removeBlock(blockLists[size]);
    }

    void BuddyAllocator::deallocate(void* addr, uint64 size)
    {
        if (addr == nullptr) return;
        if (size < BLOCK_MINIMUM_BITSIZE
            || size > BLOCK_MAXIMUM_BITSIZE) return;

        auto block = (FreeBlock*)addr;
        block->setSize(size);
        if (isBuddyFree(block))
        {
            recursiveJoinBuddies(block);
        }
        else
        {
            insertBlock(block);
        }
    }

}
