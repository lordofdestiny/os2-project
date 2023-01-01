#include "../h/kernel/Kernel.h"
#include "../h/syscall_c.h"
#include "../h/kernel/Memory/MemoryManager.h"
#include "../h/kernel/Memory/BuddyAllocator.h"
#include "../h/kernel/Memory/Cache.h"
#include "../lib/hw.h"
#include "../h/ConsoleUtils.h"
#include "../h/kernel/Utils/Utils.h"

extern void userMain();

void printMemorySection(
    const char* name,
    kernel::memory::MemorySection const& section)
{
    // Kernel::execute(userMain);
    printString(name);
    printString("_START_ADDR=");
    printAddress(section.startAddress);
    putc('\n');
    printString(name);
    printString("_END_ADDR=");
    printAddress(section.endAddress);
    putc('\n');
    printString("TOTAL_SIZE=");
    printUInt64(section.size(), 16);
    putc('\n');
    printString("----------------------------------------\n");
}

void main()
{
    using namespace kernel;
    Kernel::initialize();
    // Kernel::execute(userMain);
    /* printMemorySection("DATA", memory::dataSectionBounds());
    printMemorySection("KERNEL", memory::kernelSectionBounds());
    printMemorySection("HEAP", memory::heapSectionBounds());

    auto& allocator = memory::BuddyAllocator::getInstance();

    int pagesToAllocate = 7;
    auto first = allocator.allocate(pagesToAllocate);
    int allocs = first != nullptr ? 1 : 0;
    void* last = nullptr;
    void* temp = nullptr;
    while ((temp = allocator.allocate(pagesToAllocate)) != nullptr)
    {
        allocs++;
        last = temp;
    }
    printString("Allocations: ");
    printInt(allocs);
    putc('\n');
    printString("*************************************************************************\n");

    allocator.printBlockTable();

    printString("*************************************************************************\n");

    int deallocs = 0;
    for (auto addr = first; addr != nullptr && (uint64)addr <= (uint64)last; addr = ((char*)addr) + (PAGE_SIZE << pagesToAllocate))
    {
        deallocs++;
        allocator.deallocate(addr, pagesToAllocate);
    }
    printString("Deallocations: ");
    printInt(deallocs);
    putc('\n');

    allocator.printBlockTable(); */

    memory::Cache allocator("Test", 24, nullptr, nullptr);
    printString(allocator.name());
    printString("; SIZE= ");
    printInt(allocator.objectSize());
    printString("; OBJECT/SLAB=");
    printInt(allocator.objectsPerSlab());
    putc('\n');
    printString("*****************************************\n");
    printString("TOTAL_SIZE=");
    printInt(allocator.totalSize());
    putc('\n');
    printString("SLAB_CNT=");
    printInt(allocator.slabCount());
    putc('\n');
    printString("USAGE=");
    printInt(allocator.totalUsage());
    putc('\n');
    printString("*****************************************\n");
#define ALLOCS 200
    void* ptrs[ALLOCS];
    for (int i = 0; i < ALLOCS; i++)
    {
        ptrs[i] = allocator.allocate();
    }
    printString("TOTAL_SIZE=");
    printInt(allocator.totalSize());
    putc('\n');
    printString("SLAB_CNT=");
    printInt(allocator.slabCount());
    putc('\n');
    printString("USAGE=");
    printInt(allocator.totalUsage());
    putc('\n');
    printString("*****************************************\n");
    for (int i = 0; i < ALLOCS; i++)
    {
        allocator.deallocate(ptrs[i]);
    }
    printString("TOTAL_SIZE=");
    printInt(allocator.totalSize());
    putc('\n');
    printString("SLAB_CNT=");
    printInt(allocator.slabCount());
    putc('\n');
    printString("USAGE=");
    printInt(allocator.totalUsage());
    putc('\n');
    printString("*****************************************\n");

    Kernel::finalize();
}

