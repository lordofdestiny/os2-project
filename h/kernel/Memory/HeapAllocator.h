//
// Created by os on 7/15/22.
//
#ifndef PROJECT_MEMORYALLOCATOR_H
#define PROJECT_MEMORYALLOCATOR_H

#include "../../../lib/hw.h"
#include "../../../h/kernel/Memory/MemoryManager.h"

namespace kernel::memory::heap
{
    void initialize(MemorySection section);
    size_t bytesToBlocks(size_t bytes);
    void* alloc_blocks(size_t count);
    void* alloc_bytes(size_t count);
    int free_blocks(void* ptr);
}

#endif //PROJECT_MEMORYALLOCATOR_H
