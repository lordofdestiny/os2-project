/*
 * This class handles the logic around setting sizes of
 * the heap and kernel data storage
 */

#include "../../../lib/hw.h"


namespace kernel::memory
{
    class MemoryManager
    {
    public:
        struct MemorySection
        {
            void const* const startAddress;
            void const* const endAddress;
            MemorySection(void const* const start, void const* const end)
                : startAddress(start), endAddress(end)
            { }

            uint64 size() const;
            uint64 addressBits() const;
        };
    public:
        MemorySection dataSectionBounds() const;
        MemorySection kernelSectionBounds() const;
        MemorySection heapSectionBounds() const;
        static MemoryManager const& getInstance();
        uint64 getMinBlockSize() const;
        uint64 getMaxBlockSize() const;
    private:
        MemoryManager();
        const uint64 minBlockSize;
        const uint64 maxBlockSize;
    };

} // namespace kernel