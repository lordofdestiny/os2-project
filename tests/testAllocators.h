#include "../h/syscall_c.h"
#include "../h/kernel/Memory/MemoryManager.h"
#include "../h/kernel/Memory/BuddyAllocator.h"
#include "../h/ConsoleUtils.h"
#include "../h/kernel/Memory/slab.h"

void printMemorySection(
    const char* name,
    kernel::memory::MemorySection const& section)
{
    printString(name);
    printString(": ");
    printAddress(section.start);
    printString(" - ");
    printAddress(section.end);
    printString("\n");
    printString("----------------------------------------\n");
}

void testMain()
{
    using namespace kernel;
    printMemorySection("Memory", memory::Memory());
    printMemorySection("Code", memory::AllCode());
    printMemorySection("Kernel code", memory::KernelCode());
    printMemorySection("Kernel data", memory::KernelData());
    printMemorySection("SysteCall code", memory::SysCallCode());
    printMemorySection("SysteCall data", memory::SysCallData());
    printMemorySection("User code", memory::UserCode());
    printMemorySection("User data", memory::UserData());
    printMemorySection("User heap", memory::UserHeap());
    printMemorySection("Kernel memory", memory::KernelHeap());
    printString("*************************************************************************\n");

    memory::printBlockTable();
    int pagesToAllocate = 8;
    void* ptrs0[100];
    auto first = memory::page_alloc(pagesToAllocate);
    int allocs = first != nullptr ? 1 : 0;
    if (first != nullptr) ptrs0[0] = first;
    void* temp = nullptr;
    while ((temp = memory::page_alloc(pagesToAllocate)) != nullptr)
    {
        ptrs0[allocs++] = temp;
    }
    printString("Allocations: ");
    printInt(allocs);
    putc('\n');

    int deallocs = 0;
    for (int i = 0; i < allocs; i++)
    {
        memory::page_free(ptrs0[deallocs++], pagesToAllocate);
    }
    printString("Deallocations: ");
    printInt(deallocs);
    putc('\n');

    memory::printBlockTable();
    printString("*************************************************************************\n");

    auto cache = kmem_cache_create("Test", 36, nullptr, nullptr);
    kmem_cache_info(cache);
#define ALLOCS 100
    void* ptrs[ALLOCS];
    for (int i = 0; i < ALLOCS; i++)
    {
        ptrs[i] = kmem_cache_alloc(cache);
    }
    kmem_cache_info(cache);
    for (int i = 0; i < ALLOCS; i++)
    {
        kmem_cache_free(cache, ptrs[i]);
    }
    kmem_cache_info(cache);
    kmem_cache_shrink(cache);
    kmem_cache_info(cache);


    memory::printBlockTable();
    printString("*************************************************************************\n");

    kmem_cache_free(cache, (void*)1000);
    kmem_cache_error(cache);
    kmem_cache_free(cache, nullptr);
    kmem_cache_error(cache);

    kmalloc(1 << 20);
    if (kmem_cache_error(nullptr))
    {
        printString("Success\n");
    };
    kfree(nullptr);
    if (kmem_cache_error(nullptr))
    {
        printString("Success\n");
    }
    auto c = kmem_cache_create("should_work", 32, nullptr, nullptr);
    kmem_cache_error(nullptr);
    kmem_cache_destroy(c);
    kmem_cache_error(nullptr);

    kmem_cache_create("jakodugackoimekojecupokusatidanateramdabudeduzeod32", 100, nullptr, nullptr);
    if (kmem_cache_error(nullptr))
    {
        printString("Success\n");
    }
    kmem_cache_create("jakodugackoimek", 56, nullptr, [](void*) { });
    if (kmem_cache_error(nullptr))
    {
        printString("Success\n");
    }
    kmem_cache_create("jakodugackoimek", 1 << 18, nullptr, [](void*) { });
    if (kmem_cache_error(nullptr))
    {
        printString("Success\n");
    }

    memory::printBlockTable();
    printString("*********************************************\n");

    auto big_buff = kmem_cache_create("Big Daddy", 1 << 7, nullptr, nullptr);
    kmem_cache_info(big_buff);
    kmem_cache_error(big_buff);
    kmem_cache_error(nullptr);
    printString("*********************************************\n");


    auto ptr = kmem_cache_alloc(big_buff);
    printAddress(ptr);
    putc('\n');
    kmem_cache_info(big_buff);
    kmem_cache_error(big_buff);
    auto ptr1 = kmem_cache_alloc(big_buff);
    printAddress(ptr1);
    putc('\n');
    kmem_cache_info(big_buff);
    kmem_cache_error(big_buff);
    auto ptr2 = kmem_cache_alloc(big_buff);
    printAddress(ptr2);
    putc('\n');
    kmem_cache_info(big_buff);
    kmem_cache_error(big_buff);
    printString("*********************************************\n");
    memory::printBlockTable();

    kmem_cache_free(big_buff, ptr);
    kmem_cache_info(big_buff);
    kmem_cache_error(big_buff);
    printString("*********************************************\n");

    kmem_cache_free(big_buff, ptr1);
    kmem_cache_info(big_buff);
    kmem_cache_error(big_buff);
    printString("*********************************************\n");

    kmem_cache_free(big_buff, ptr2);
    kmem_cache_info(big_buff);
    kmem_cache_error(big_buff);
    printString("*********************************************\n");

    kmem_cache_shrink(big_buff);
    kmem_cache_info(big_buff);
    kmem_cache_error(big_buff);

    printString("*********************************************\n");
    memory::printBlockTable();
}