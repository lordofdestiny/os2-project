#include "../../../h/kernel/Memory/MemoryManager.h"
#include "../../../h/kernel/Utils/Utils.h"
#include "../../../lib/hw.h"

namespace kernel::memory
{
    uint64 MemorySection::size() const
    {
        return (uint64)endAddress - (uint64)startAddress;
    }

    MemorySection dataSectionBounds()
    {
        return {
            (void*)HEAP_START_ADDR,
            (void*)HEAP_END_ADDR
        };
    }

    MemorySection kernelSectionBounds()
    {
        return {
            (char*)(MEMORY_SECOND_HALF)
            +7 * (((uint64)HEAP_END_ADDR - MEMORY_SECOND_HALF) >> 3),
            (void*)HEAP_END_ADDR
        };
    }

    MemorySection heapSectionBounds()
    {
        return {
            (void*)HEAP_START_ADDR,
            (char*)(MEMORY_SECOND_HALF)
            +7 * (((uint64)HEAP_END_ADDR - MEMORY_SECOND_HALF) >> 3)
        };
    }

} // namespace kernel::memory
