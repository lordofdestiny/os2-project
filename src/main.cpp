#include "../h/kernel/Kernel.h"
#include "../h/syscall_c.h"
#include "../h/kernel/Memory/MemoryManager.h"
#include "../h/kernel/Memory/BuddyAllocator.h"
#include "../lib/hw.h"
#include "../h/ConsoleUtils.h"
#include "../h/kernel/Utils/Math.h"

extern void userMain();

void main()
{
    using namespace kernel;
    Kernel::initialize();
    // Kernel::execute(userMain);
    auto dataBounds = memory::dataSectionBounds();
    printString("DATA_START_ADDR=");
    printAddress(dataBounds.startAddress);
    putc('\n');
    printString("DATA_END_ADDR=");
    printAddress(dataBounds.endAddress);
    putc('\n');
    printString("TOTAL_SIZE=");
    printUInt64(dataBounds.size(), 16);
    putc('\n');
    printString("----------------------------------------\n");

    auto kernelBounds = memory::kernelSectionBounds();
    printString("KERNEL_START_ADDR=");
    printAddress(kernelBounds.startAddress);
    putc('\n');
    printString("KERNEL_END_ADDR=");
    printAddress(kernelBounds.endAddress);
    putc('\n');
    printString("TOTAL_SIZE=");
    printUInt64(kernelBounds.size(), 16);
    putc('\n');
    printString("----------------------------------------\n");

    auto heapBounds = memory::heapSectionBounds();
    printString("HEAP_START_ADDR=");
    printAddress(heapBounds.startAddress);
    putc('\n');
    printString("HEAP_END_ADDR=");
    printAddress(heapBounds.endAddress);
    putc('\n');
    printString("TOTAL_SIZE=");
    printUInt64(heapBounds.size(), 16);
    putc('\n');
    printString("----------------------------------------\n");

    Kernel::finalize();
}

