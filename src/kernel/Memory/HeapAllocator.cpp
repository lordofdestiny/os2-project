//
// Created by os on 7/15/22.
//

#include "../../../h/kernel/Memory/HeapAllocator.h"
#include "../../../h/kernel/Utils/Utils.h"

namespace kernel::memory::heap
{
    struct FreeBlock
    {
        FreeBlock* prev, * next;
        size_t size;

        FreeBlock() = delete;
        FreeBlock(FreeBlock const&) = delete;
        FreeBlock& operator=(FreeBlock const&) = delete;
        ~FreeBlock() = delete;
        FreeBlock(FreeBlock* other):
            prev(other->prev),
            next(other->next),
            size(other->size)
        { }

        static void* operator new(size_t, void* ptr)
        {
            return ptr;
        }

        char* end()
        {
            return (char*)this + size * MEM_BLOCK_SIZE;
        }
    };

    static FreeBlock* head;

    void initialize(MemorySection section)
    {
        const auto heapByteSize = section.size();
        const auto heapBlockCount =
            heapByteSize / MEM_BLOCK_SIZE;

        head = (FreeBlock*)section.start;
        head->size = heapBlockCount;
        head->prev = nullptr;
        head->next = nullptr;
    }

    size_t bytesToBlocks(size_t blocks)
    {
        return (blocks + sizeof(size_t) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    }

    /* Algorithm: Best Fit*/
    void* alloc_blocks(size_t count)
    {
        FreeBlock* block = nullptr;

        for (auto curr = head; curr != nullptr; curr = curr->next)
        {
            if ((block == nullptr && curr->size >= count) ||
                (count <= curr->size && curr->size < block->size))
            {
                block = curr;
            }
        }

        if (block == nullptr) return nullptr;

        const auto remainingBlocks = block->size - count;
        const auto offset = count * MEM_BLOCK_SIZE;
        auto toUpdate = remainingBlocks > 0
            ? (FreeBlock*)((char*)block + offset)
            : block->next;

        if (block->prev == nullptr)
        {
            head = toUpdate;
        }
        else
        {
            block->prev->next = toUpdate;
        }
        if (block->next != nullptr)
        {
            block->next->prev = toUpdate;
        }

        if (remainingBlocks > 0)
        {
            toUpdate->prev = block->prev;
            toUpdate->next = block->next;
            toUpdate->size = remainingBlocks;
        }

        *((size_t*)block) = count; // Upisi velicinu

        return (char*)block + sizeof(size_t);
    }

    void* alloc_bytes(size_t count)
    {
        const auto blockCount = bytesToBlocks(count);
        return alloc_blocks(blockCount);
    }

    int free_blocks(void* ptr)
    {
        if (ptr == nullptr) return -1;

        const auto addr = (char*)ptr - sizeof(size_t);

        // If ptr was not in heap range
        if (addr < HEAP_START_ADDR ||
            addr >= HEAP_END_ADDR)
        {
            return -2;
        }

        const auto blockCount = *((size_t*)addr);
        const auto byteSize = blockCount * MEM_BLOCK_SIZE;

        // If end of ptr is after end of the heap
        if ((char*)addr + byteSize >= HEAP_END_ADDR)
        {
            return  -3;
        }

        // Find the place where to insert new free segment
        // Insertion happens after the block pointed to by curr
        auto curr = head;
        if (head == nullptr || addr < (char*)head)
        {
            curr = nullptr; // Insert as the first
        }
        else
        {
            while (curr->next != nullptr
                && addr > (char*)(curr->next))
            {
                curr = curr->next;
            }
        }

        // Try to append block to the previous free segment curr
        if (curr != nullptr && curr->end() == addr)
        {
            curr->size += blockCount;
            // Try to join curr with the next free ptr
            if (curr->next != nullptr &&
                curr->end() == (char*)(curr->next))
            {
                curr->size += curr->next->size;
                curr->next = curr->next->next;
                if (curr->next != nullptr)
                {
                    curr->next->prev = curr;
                }
            }
            return 0;
        }

        // Try to append it to the next free segment
        auto nextSeg = curr != nullptr ? curr->next : head;
        if (nextSeg != nullptr &&
            (char*)nextSeg == addr + byteSize)
        {
            auto newSeg = new(addr) FreeBlock(nextSeg);
            newSeg->size += blockCount;
            if (nextSeg->next != nullptr)
            {
                nextSeg->next->prev = newSeg;
            }
            if (nextSeg->prev == nullptr) head = newSeg;
            else nextSeg->prev->next = newSeg;

            return 0;
        }

        // No need to join; insert the new ptr after curr
        auto newSeg = (FreeBlock*)addr;
        newSeg->prev = curr;
        newSeg->size = blockCount;
        newSeg->next = curr != nullptr
            ? curr->next
            : head;

        if (newSeg->next != nullptr)
        {
            newSeg->next->prev = newSeg;
        }

        if (curr == nullptr) head = newSeg;
        else curr->next = newSeg;

        return 0;
    }
}