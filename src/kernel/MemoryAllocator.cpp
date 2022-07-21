//
// Created by os on 7/15/22.
//

#include "../../h/kernel/MemoryAllocator.h"
#include "../../h/kernel/ConsoleUtils.h"

namespace kernel {

    MemoryAllocator::MemoryAllocator() {
        auto heapByteSize = (size_t) HEAP_END_ADDR - (size_t) HEAP_START_ADDR;
        auto heapBlockCount = heapByteSize / MEM_BLOCK_SIZE;
        head = new((FreeBlock *) HEAP_START_ADDR) FreeBlock(heapBlockCount);
    }

    MemoryAllocator &MemoryAllocator::getInstance() {
        static MemoryAllocator allocatorInstance{};
        return allocatorInstance;
    }

    size_t MemoryAllocator::byteSizeToBlockCount(size_t blocks) {
        return (blocks + sizeof(size_t) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    }

/* Algorithm: Best Fit*/
    void *MemoryAllocator::allocateBlocks(size_t count) {
        FreeBlock *block = nullptr;

        for (auto curr = head; curr != nullptr; curr = curr->next) {
            if ((block == nullptr && curr->size >= count)
                || (curr->size >= count && curr->size < block->size)) {
                block = curr;
            }
        }

        if (block == nullptr) return nullptr;

        auto remainingBlocks = block->size - count;
        /* If the current segment is not emtpy after being consumed
         * Use required space and create new block that represent
         * remaining space
         * */
        if (remainingBlocks > 0) {
            auto offset = count * MEM_BLOCK_SIZE;
            auto newBlock = (FreeBlock *) ((char *) block + offset);

            block->prev != nullptr
            ? block->prev->next
            : head
                      = newBlock;

            if (block->next != nullptr) {
                block->next->prev = newBlock;
            }

            new(newBlock) FreeBlock(*block);
            newBlock->size = remainingBlocks;
        } else {
            /* Consume the entire current block, and
             * then remove it
             * */
            block->prev != nullptr
            ? block->prev->next
            : head
                      = block->next;

            if (block->next != nullptr) {
                block->next->prev = block->prev;
            }
        }

        block->next = block->prev = nullptr;
        *((size_t *) block) = count; // Upisi velicinu

        return (char *) block + sizeof(size_t);
    }

    int MemoryAllocator::deallocateBlocks(void *ptr) {
        if (ptr == nullptr) return -1;

        auto address = (char *) ptr - sizeof(size_t);

        // If ptr was not in heap range
        if (HEAP_START_ADDR > address || address >= HEAP_END_ADDR) {
            return -2;
        }

        auto blockCount = *((size_t *) address);
        auto byteSize = blockCount * MEM_BLOCK_SIZE;

        // Find the place where to insert new free segment
        // Insertion happens after the block pointed to by curr
        FreeBlock *curr = head;
        if (head == nullptr || address < (char *) head) {
            curr = nullptr; // Insert as the first
        } else {
            while (curr->next != nullptr && address > (char *) (curr->next))
                curr = curr->next;
        }

        // Try to append block to the previous free segment curr
        if (curr != nullptr && (char *) curr->end() == address) {
            curr->size += blockCount;
            // Try to join curr with the next free ptr
            if (curr->next != nullptr && curr->end() == (char *) (curr->next)) {
                curr->size += curr->next->size;
                curr->next = curr->next->next;
                if (curr->next != nullptr) curr->next->prev = curr;
            }
            return 0;
        } else {
            // Try to append it to the next free segment
            auto *nextSeg = curr ? curr->next : head;
            if (nextSeg != nullptr && address + byteSize == (char *) nextSeg) {
                auto newSeg = new((FreeBlock *) address) auto(*nextSeg);
                newSeg->size += blockCount;

                if (nextSeg->next != nullptr) {
                    nextSeg->next->prev = newSeg;
                }

                newSeg->next == nullptr
                ? head
                : nextSeg->prev->next
                          = nextSeg;

                return 0;
            }
        }

        // No need to join; insert the new ptr after curr
        auto newSeg = new ((FreeBlock *) address) FreeBlock(blockCount);
        newSeg->prev = curr;
        newSeg->next = curr == nullptr
                       ? head
                       : curr->next;

        if (newSeg->next != nullptr) {
            newSeg->next->prev = newSeg;
        }

        curr == nullptr
        ? head
        : curr->next
                  = newSeg;

        return 0;
    }

    void *MemoryAllocator::allocateBytes(size_t byteCount) {
        auto blockCount = byteSizeToBlockCount(byteCount);
        return allocateBlocks(blockCount);
    }
}