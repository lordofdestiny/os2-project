//
// Created by os on 7/15/22.
//

#include "../../h/kernel/MemoryAllocator.h"

namespace kernel {
    MemoryAllocator::MemoryAllocator() {
        auto heapByteSize = (uint64)HEAP_END_ADDR - (uint64)HEAP_START_ADDR;
        auto heapBlockCount = heapByteSize / MEM_BLOCK_SIZE;
        auto originBlock = (FreeBlock*)HEAP_START_ADDR;
        originBlock->size = heapBlockCount;
        originBlock->prev = nullptr;
        originBlock->next = nullptr;
        this->head = originBlock;
    }

    MemoryAllocator& MemoryAllocator::getInstance() {
        static MemoryAllocator instance{};
        return instance;
    }

    size_t MemoryAllocator::byteSizeToBlockCount(size_t blocks) {
        return (blocks + 2 * sizeof(size_t) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    }

    /* Algorithm: Best Fit*/
    void* MemoryAllocator::allocateBlocks(size_t count) {
        FreeBlock* block = nullptr;

        for (auto curr = head; curr != nullptr; curr = curr->next) {
            if (block == nullptr && curr->size >= count) {
                block = curr;
            } else if (curr->size >= count && curr->size < block->size) {
                block = curr;
            }
        }

        if (block == nullptr) return nullptr;

        auto remainingBlocks = block->size - count;
        if (remainingBlocks > 0) {
            auto offset = count * MEM_BLOCK_SIZE;
            auto newBlock = (FreeBlock*)((char*)block + offset);

            if (block->prev != nullptr) {
                block->prev->next = newBlock;
            } else {
                head = newBlock;
            }
            if (block->next != nullptr) {
                block->next->prev = newBlock;
            }
            newBlock->prev = block->prev;
            newBlock->next = block->next;
            newBlock->size = remainingBlocks;
        } else {
            if (block->prev != nullptr) {
                block->prev->next = block->next;
            } else {
                head = block->next;
            }
            if (block->next != nullptr) {
                block->next->prev = block->prev;
            }
        }

        *((size_t*)block) = count; // Upisi velicinu

        return (char*)block + 2 * sizeof(size_t);
    }

    int MemoryAllocator::deallocateBlocks(void* ptr) {
        if (ptr == nullptr) return -1;

        auto address = (char*)ptr - 2 * sizeof(size_t);

        // If ptr was not in heap range
        if (HEAP_START_ADDR > address || address >= HEAP_END_ADDR) {
            return -2;
        }

        auto blockCount = *((size_t*)address);
        auto byteSize = blockCount * MEM_BLOCK_SIZE;

        // If end of ptr is after end of the heap
        if ((char*)address + byteSize >= HEAP_END_ADDR) {
            return  -3;
        }

        // Find the place where to insert new free segment
        // Insertion happens after the block pointed to by curr
        auto curr = head;
        if (head == nullptr || address < (char*)head) {
            curr = nullptr; // Insert as the first
        } else {
            while (curr->next != nullptr && address > (char*)(curr->next)) {
                curr = curr->next;
            }
        }

        // Try to append block to the previous free segment curr
        if (curr != nullptr && curr->end() == address) {
            curr->size += blockCount;
            // Try to join curr with the next free ptr
            if (curr->next != nullptr && curr->end() == (char*)(curr->next)) {
                curr->size += curr->next->size;
                curr->next = curr->next->next;
                if (curr->next != nullptr) curr->next->prev = curr;
            }
            return 0;
        } else {
            // Try to append it to the next free segment
            auto nextSeg = curr ? curr->next : head;
            if (nextSeg != nullptr && address + byteSize == (char*)nextSeg) {
                auto newSeg = (FreeBlock*)address;
                newSeg->prev = nextSeg->prev;
                newSeg->next = nextSeg->next;
                newSeg->size = blockCount + nextSeg->size;
                if (nextSeg->next != nullptr) {
                    nextSeg->next->prev = newSeg;
                }
                if (nextSeg->prev != nullptr) {
                    nextSeg->prev->next = newSeg;
                } else {
                    head = newSeg;
                }
                return 0;
            }
        }

        // No need to join; insert the new ptr after curr
        auto newSeg = (FreeBlock*)address;
        newSeg->prev = curr;
        newSeg->size = blockCount;
        if (curr != nullptr) {
            newSeg->next = curr->next;
        } else {
            newSeg->next = head;
        }
        if (newSeg->next != nullptr) {
            newSeg->next->prev = newSeg;
        }
        if (curr != nullptr) {
            curr->next = newSeg;
        } else {
            head = newSeg;
        }

        return 0;
    }

    void* MemoryAllocator::allocateBytes(size_t count) {
        auto blockCount = byteSizeToBlockCount(count);
        return allocateBlocks(blockCount);
    }
}