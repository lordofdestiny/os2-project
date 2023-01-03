#include "../../../h/kernel/Memory/BuddyAllocator.h"
#include "../../../h/kernel/Memory/MemoryManager.h"
#include "../../../h/kernel/Utils/Utils.h"
#include "../../../h/ConsoleUtils.h"
#include "../../../lib/hw.h"

namespace kernel::memory
{
    size_t MemorySection::size() const
    {
        return (size_t)endAddress - (size_t)startAddress;
    }

    void* nextPage(void const* page)
    {
        return (void*)(((uint64)(page) & ((uint64)-1 << (PAGE_ORDER - 1))) + PAGE_SIZE);
    }

    MemorySection RAMSection()
    {
        return {
            (void*)MEMORY_SECOND_HALF,
            (void*)HEAP_END_ADDR
        };
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
            (char*)(MEMORY_SECOND_HALF)+7 * (RAMSection().size() >> 3),
            (void*)HEAP_END_ADDR
        };
    }

    MemorySection heapSectionBounds()
    {
        return {
            (void*)nextPage(HEAP_START_ADDR),
            (char*)(MEMORY_SECOND_HALF)+7 * (RAMSection().size() >> 3)
        };
    }

} // namespace kernel::memory
