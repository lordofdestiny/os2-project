//
// Created by os on 7/15/22.
//

#include "../../h/kernel/MemoryAllocator.h"

namespace kernel {
    MemoryAllocator::MemoryAllocator() {
        size_t heapByteSize = (uint64) HEAP_END_ADDR - (uint64) HEAP_START_ADDR;
        size_t heapBlockCount = heapByteSize / MEM_BLOCK_SIZE;
        auto originBlock = (MemoryAllocator::FreeMemoryBlock *) HEAP_START_ADDR;
        originBlock->size = heapBlockCount - sizeof(FreeMemoryBlock);
        originBlock->prev = nullptr;
        originBlock->next = nullptr;
        this->freeMemoryHead = originBlock;
    }

    MemoryAllocator &MemoryAllocator::getInstance() {
        static MemoryAllocator allocatorInstance{};
        return allocatorInstance;
    }

    size_t MemoryAllocator::byteSizeToBlockCount(size_t blocks) {
        return (blocks + sizeof(size_t) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    }

/* Algorithm: Best Fit*/
    void *MemoryAllocator::allocateBlocks(size_t blockCount) {
        FreeMemoryBlock *block = nullptr;

        for (auto curr = this->freeMemoryHead; curr != nullptr; curr = curr->next) {
            if (block == nullptr && curr->size >= blockCount) {
                block = curr;
            } else if (curr->size >= blockCount && curr->size < block->size) {
                block = curr;
            }
        }

        if (block == nullptr) {
            return nullptr;
        }

        size_t remainingBlockCount = block->size - blockCount;
        if (remainingBlockCount > 0) {
            size_t offset = blockCount * MEM_BLOCK_SIZE;
            auto newBlock = (FreeMemoryBlock*) ((char*) block + offset);

            if (block->prev) {
                block->prev->next = newBlock;
            } else {
                freeMemoryHead = newBlock;
            }
            if(block->next){
                block->next->prev = newBlock;
            }
            newBlock->prev = block->prev;
            newBlock->next = block->next;
            newBlock->size = remainingBlockCount;
        } else {
            if (block->prev) {
                block->prev->next = block->next;
            } else {
                freeMemoryHead = block->next;
            }
            if(block->next) {
                block->next->prev = block->prev;
            }
        }

        block->next = nullptr;
        block->prev = nullptr;
        *((size_t*)block) = blockCount; // Upisi velicinu

        return (char *) block + sizeof(size_t);
    }

    int MemoryAllocator::deallocateBlocks(void* address) {
        char* blockAddress = (char*)address - sizeof(size_t);

        // If address was not in heap range
        if(HEAP_START_ADDR > blockAddress || blockAddress >= HEAP_END_ADDR){
            return -1;
        }

        size_t blockCount = *((size_t*) blockAddress);
        size_t blocksSizeBytes = blockCount * MEM_BLOCK_SIZE;

        // If end of address is after end of the heap
        if((char*)blockAddress + blocksSizeBytes >= HEAP_END_ADDR) {
            return  -2;
        }

        // Find the place where to insert new free segment
        // Insertion happens after the block pointed to by curr
        FreeMemoryBlock* curr = nullptr;
        if(!freeMemoryHead || blockAddress < (char*) freeMemoryHead){
            curr = nullptr; // Insert as the first
        }else{
            for(curr=freeMemoryHead; curr->next != nullptr && blockAddress > (char*)(curr->next); curr=curr->next);
        }

        // Try to append block to the previous free segment curr
        if(curr && (char*)curr + curr->size * MEM_BLOCK_SIZE == blockAddress){
            curr->size += blockCount;
            // Try to join curr with the next free address
            if(curr->next && (char*)curr+curr->size * MEM_BLOCK_SIZE  == (char*)(curr->next)){
                curr->size += curr->next->size;
                curr->next = curr->next->next;
                if(curr->next) curr->next->prev = curr;
            }
            return 0;
        }else {
            // Try to append it to the next free segment
            FreeMemoryBlock* nextSeg = curr?curr->next:freeMemoryHead;
            if(nextSeg && (char*)blockAddress + blocksSizeBytes == (char*)nextSeg){
                auto newSeg = (FreeMemoryBlock*)blockAddress;
                newSeg->size = blockCount + nextSeg->size;
                newSeg->prev = nextSeg->prev;
                newSeg->next = newSeg->next;
                if(nextSeg->next){
                    nextSeg->next->prev = newSeg;
                }
                if(nextSeg->prev) {
                    nextSeg->prev->next = newSeg;
                }else{
                    freeMemoryHead = newSeg;
                }
                return 0;
            }
        }

        // No need to join; insert the new address after curr
        auto newSeg = (FreeMemoryBlock*) blockAddress;
        newSeg->size = blockCount;
        newSeg->prev = curr;
        if(curr) {
            newSeg->next = curr->next;
        }else{
            newSeg->next = freeMemoryHead;
        }
        if(newSeg->next) {
            newSeg ->next->prev = newSeg;
        }
        if(curr) {
            curr->next = newSeg;
        }else{
            freeMemoryHead = newSeg;
        }

        return 0;
    }

    void *MemoryAllocator::allocateBytes(size_t byteCount) {
        auto blockCount = byteSizeToBlockCount(byteCount);
        return allocateBlocks(blockCount);
    }
}