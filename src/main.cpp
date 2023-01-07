#include "../h/kernel/Kernel.h"
#include "../h/syscall_c.h"
#include "../h/ConsoleUtils.h"
#include "../h/kernel/Memory/MemoryManager.h"
#include "../h/kernel/Utils/BitMasks.h"

extern void userMain();

void main()
{
    using namespace kernel;
    Kernel::initialize();

    printString("Memory: ");
    printAddress(kernel::memory::Memory().start);
    printString(" - ");
    printAddress(kernel::memory::Memory().end);
    printString("\n");
    printString("*************************************************\n");

    printString("Code: ");
    printAddress(kernel::memory::AllCode().start);
    printString(" - ");
    printAddress(kernel::memory::AllCode().end);
    printString("\n");
    printString("*************************************************\n");

    printString("Kernel code: ");
    printAddress(kernel::memory::KernelCode().start);
    printString(" - ");
    printAddress(kernel::memory::KernelCode().end);
    printString("\n");
    printString("*************************************************\n");

    printString("Kernel data: ");
    printAddress(kernel::memory::KernelData().start);
    printString(" - ");
    printAddress(kernel::memory::KernelData().end);
    printString("\n");
    printString("*************************************************\n");

    printString("SysteCall code: ");
    printAddress(kernel::memory::SysCallCode().start);
    printString(" - ");
    printAddress(kernel::memory::SysCallCode().end);
    printString("\n");
    printString("*************************************************\n");

    printString("SysteCall data: ");
    printAddress(kernel::memory::SysCallData().start);
    printString(" - ");
    printAddress(kernel::memory::SysCallData().end);
    printString("\n");
    printString("*************************************************\n");

    printString("User code: ");
    printAddress(kernel::memory::UserCode().start);
    printString(" - ");
    printAddress(kernel::memory::UserCode().end);
    printString("\n");
    printString("*************************************************\n");

    printString("User data: ");
    printAddress(kernel::memory::UserData().start);
    printString(" - ");
    printAddress(kernel::memory::UserData().end);
    printString("\n");
    printString("*************************************************\n");

    printString("User heap: ");
    printAddress(kernel::memory::UserHeap().start);
    printString(" - ");
    printAddress(kernel::memory::UserHeap().end);
    printString("\n");
    printString("*************************************************\n");

    printString("Kernel memory: ");
    printAddress(kernel::memory::KernelHeap().start);
    printString(" - ");
    printAddress(kernel::memory::KernelHeap().end);
    printString("\n");
    printString("*************************************************\n");

    Kernel::execute(userMain);

    Kernel::finalize(); // Add disposing of all caches via the API?
}

