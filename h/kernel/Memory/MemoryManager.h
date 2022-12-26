/*
 * This class handles the logic around setting sizes of
 * the heap and kernel data storage
 */

#ifndef PROJECT_MEMORYMANAGER_H
#define PROJECT_MEMORYMANAGER_H
#include "../../../lib/hw.h"

constexpr uint64 MEMORY_SECOND_HALF = 1ull << 31;


namespace kernel::memory
{
    struct MemorySection
    {
        void const* const startAddress;
        void const* const endAddress;
        MemorySection(void const* const start, void const* const end)
            : startAddress(start), endAddress(end)
        { }

        uint64 size() const;
    };

    MemorySection dataSectionBounds();
    MemorySection kernelSectionBounds();
    MemorySection heapSectionBounds();

} // namespace kernel

#endif