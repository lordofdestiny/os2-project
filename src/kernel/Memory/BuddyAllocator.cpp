#include "../../../h/kernel/Memory/BuddyAllocator.h"
#include "../../../h/kernel/Memory/MemoryManager.h"
#include "../../../h/ConsoleUtils.h"
#include "../../../h/kernel/Utils/Utils.h"

namespace kernel::memory
{
    inline void BuddyAllocator::FreeBlock::setOrder(unsigned int order)
    {
        this->order = order;
    }

    inline auto BuddyAllocator::FreeBlock::getBuddy() const
        ->FreeBlock*
    {
        return (FreeBlock*)((uint64)this ^ (1 << order));
    }

    BuddyAllocator::BuddyAllocator(
        MemorySection const& section
    )
    {
        for (size_t i = 0; i < BLOCK_LISTS_SIZE; i++)
        {
            blockLists[i] = nullptr;
        }

        const auto max = kernel::utils::log2(section.size());
        (*this)[max] = (FreeBlock*)section.startAddress;
        (*this)[max]->prev = nullptr;
        (*this)[max]->next = nullptr;
        (*this)[max]->setOrder(max);
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
            || (*this)[block->order] == nullptr)
        {
            return nullptr;
        }

        if ((*this)[block->order] == block)
        {
            (*this)[block->order] = block->next;
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
        auto head = (*this)[block->order];
        if (head != nullptr)
        {
            head->prev = block;
        }
        block->next = head;
        block->prev = nullptr;
        (*this)[block->order] = block;
    }

    // Try to split until 2^size block is found
    bool BuddyAllocator::trySplitInto(unsigned int order)
    {
        // Required block already exists, no need to split
        if ((*this)[order] != nullptr) return true;

        auto corder = order;
        while (corder <= MAX_ORDER
            && (*this)[corder] == nullptr)
        {
            corder++;
        }

        if (corder > MAX_ORDER) return false;

        while (corder > order)
        {
            auto block1 = removeBlock((*this)[corder]);
            corder -= 1;
            block1->setOrder(corder);
            auto block2 = block1->getBuddy();
            block2->setOrder(corder);

            insertBlock(block2);
            insertBlock(block1);
        }

        return true;
    }

    bool  BuddyAllocator::isBuddyFree(FreeBlock* block)
    {
        auto buddy = block->getBuddy();

        auto curr = (*this)[block->order];
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
            current->order++;
        } while (isBuddyFree(current));
        insertBlock(current);
    }

    void* BuddyAllocator::allocate(unsigned int order)
    {
        const auto blockOrder = MIN_ORDER + order;

        if (blockOrder < MIN_ORDER || blockOrder > MAX_ORDER)
        {
            return nullptr;
        }

        if (!trySplitInto(blockOrder)) return nullptr;

        return removeBlock((*this)[blockOrder]);
    }

    void BuddyAllocator::deallocate(void* addr, unsigned int order)
    {
        if (addr == nullptr) return;

        if ((uint64)start_address > (uint64)addr
            || (uint64)addr >= (uint64)end_address) return;

        const auto blockOrder = MIN_ORDER + order;

        if (blockOrder < MIN_ORDER || blockOrder > MAX_ORDER)
        {
            return;
        }

        auto block = (FreeBlock*)addr;
        block->setOrder(blockOrder);
        if (isBuddyFree(block))
        {
            recursiveJoinBuddies(block);
        }
        else
        {
            insertBlock(block);
        }
    }

    void BuddyAllocator::printBlockTable()
    {
        for (int i = 0; i < BLOCK_LISTS_SIZE; i++)
        {
            printInt(i + MIN_ORDER);
            printString(". ");
            printAddress(blockLists[i]);
            printString("\n");
        }
    }

    inline auto BuddyAllocator::operator[](size_t i)
        -> FreeBlock*&
    {
        return blockLists[i - MIN_ORDER];
    }

}
