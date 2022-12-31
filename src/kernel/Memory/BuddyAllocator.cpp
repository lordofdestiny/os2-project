#include "../../../h/kernel/Memory/BuddyAllocator.h"
#include "../../../h/kernel/Memory/MemoryManager.h"
#include "../../../h/ConsoleUtils.h"
#include "../../../h/kernel/Utils/Utils.h"

namespace kernel::memory
{
    inline void BuddyAllocator::FreeBlock::setSize(uint64 bits)
    {
        this->bits = bits;
    }

    inline auto BuddyAllocator::FreeBlock::getBuddy() const
        ->FreeBlock*
    {
        return (FreeBlock*)((uint64)this ^ (1 << bits));
    }

    inline uint64
        BuddyAllocator::FreeBlock::sizeToBits(uint64 size)
    {
        const auto log = log2(size);

        printUInt64(size == (1ull << log)
            ? log
            : log + 1);
        printString("\n");

        return size == (1ull << log)
            ? log
            : log + 1;
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
            || blockLists[block->bits] == nullptr)
        {
            return nullptr;
        }

        if (blockLists[block->bits] == block)
        {
            blockLists[block->bits] = block->next;
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
        auto head = blockLists[block->bits];
        if (head != nullptr)
        {
            head->prev = block;
        }
        block->next = head;
        block->prev = nullptr;
        blockLists[block->bits] = block;
    }

    // Try to split until 2^size block is found
    bool BuddyAllocator::trySplitInto(uint64 bits)
    {
        // Required block already exists, no need to split
        if (blockLists[bits] != nullptr) return true;

        auto cbits = bits;
        while (cbits < BLOCK_LISTS_SIZE
            && blockLists[cbits] == nullptr)
        {
            cbits++;
        }

        if (cbits == BLOCK_LISTS_SIZE) return false;

        while (cbits > bits)
        {
            auto block1 = removeBlock(blockLists[cbits]);
            cbits -= 1;
            block1->setSize(cbits);
            auto block2 = block1->getBuddy();
            block2->setSize(cbits);

            insertBlock(block2);
            insertBlock(block1);
        }

        return true;
    }

    bool  BuddyAllocator::isBuddyFree(FreeBlock* block)
    {
        auto buddy = block->getBuddy();

        auto curr = blockLists[block->bits];
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
            current->bits++;
        } while (isBuddyFree(current));
        insertBlock(current);
    }

    void* BuddyAllocator::allocate(uint64 size)
    {
        const auto bitsize = FreeBlock::sizeToBits(size);

        if (bitsize < BLOCK_MINIMUM_BITSIZE
            || bitsize > BLOCK_MAXIMUM_BITSIZE)
        {
            return nullptr;
        }

        if (!trySplitInto(bitsize)) return nullptr;

        return removeBlock(blockLists[bitsize]);
    }

    void BuddyAllocator::deallocate(void* addr, uint64 size)
    {
        if (addr == nullptr) return;

        const auto bitsize = FreeBlock::sizeToBits(size);

        auto block = (FreeBlock*)addr;
        block->setSize(bitsize);
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
