#include "../../../h/kernel/Memory/MemoryManager.h"
#include "../../../h/kernel/Utils/Math.h"
#include "../../../lib/hw.h"

namespace kernel::memory
{
    uint64 MemoryManager::MemorySection::size() const
    {
        return (uint64)endAddress - (uint64)startAddress;
    }

    uint64 MemoryManager::MemorySection::addressBits() const
    {
        return log2(size() - 1);
    }

    MemoryManager::MemoryManager()
        : minBlockSize(5),
        maxBlockSize(log2(dataSectionBounds().size()))
    { }

    MemoryManager const&
        MemoryManager::getInstance()
    {
        static MemoryManager instance{};
        return instance;
    }

    auto MemoryManager::dataSectionBounds() const
        -> MemorySection
    {
        return {
            HEAP_START_ADDR,
            (char*)HEAP_END_ADDR
        };
    }

    auto MemoryManager::kernelSectionBounds() const
        -> MemorySection
    {
        return {
            HEAP_START_ADDR,
            (char*)HEAP_START_ADDR
            + ((2ull << (maxBlockSize - 3)))
        };
    }

    auto MemoryManager::heapSectionBounds() const
        -> MemorySection
    {
        return {
            (char*)HEAP_START_ADDR +
            kernelSectionBounds().size(),
            HEAP_END_ADDR
        };
    }

    uint64  MemoryManager::getMinBlockSize() const
    {
        return minBlockSize;
    }
    uint64  MemoryManager::getMaxBlockSize() const
    {
        return maxBlockSize;
    }
} // namespace kernel::memory
