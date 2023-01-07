/*
 * This class handles the logic around setting sizes of
 * the heap and kernel data storage
 */

#ifndef PROJECT_MEMORYMANAGER_H
#define PROJECT_MEMORYMANAGER_H
#include "../../../lib/hw.h"

namespace kernel::memory
{
    struct MemorySection
    {
        void* const start;
        void* const end;
        inline MemorySection(void* start, void* end)
            : start(start), end(end)
        { }

        size_t size() const;
    };
    MemorySection AllCode();
    MemorySection KernelCode();
    MemorySection SysCallCode();
    MemorySection KernelData();
    MemorySection UserCode();
    MemorySection SysCallData();
    MemorySection UserData();

    MemorySection Memory();
    MemorySection KernelHeap();
    MemorySection UserHeap();
} // namespace kernel

#endif