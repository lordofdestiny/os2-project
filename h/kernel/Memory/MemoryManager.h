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
        void* const startAddress;
        void* const endAddress;
        MemorySection(void* const start, void* const end)
            : startAddress(start), endAddress(end)
        { }

        size_t size() const;
    };

    void* nextPage(void const* page);

    MemorySection RAMSection();
    MemorySection dataSectionBounds();
    MemorySection kernelSectionBounds();
    MemorySection heapSectionBounds();
} // namespace kernel

#endif