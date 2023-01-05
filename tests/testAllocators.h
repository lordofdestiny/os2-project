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

void testMain()
{
    using namespace kernel;
    printMemorySection("DATA", memory::dataSectionBounds());
    printMemorySection("KERNEL", memory::kernelSectionBounds());
    printMemorySection("HEAP", memory::heapSectionBounds());
    printString("*************************************************************************\n");

    auto& allocator = memory::BLOCKS;
    allocator.printBlockTable();
    memory::MemoryErrorManager em;
    int pagesToAllocate = 8;
    void* ptrs0[100];
    auto first = allocator.allocate(pagesToAllocate, em);
    int allocs = first != nullptr ? 1 : 0;
    if (first != nullptr) ptrs0[0] = first;
    void* temp = nullptr;
    while ((temp = allocator.allocate(pagesToAllocate, em)) != nullptr)
    {
        ptrs0[allocs++] = temp;
    }
    printString("Allocations: ");
    printInt(allocs);
    putc('\n');

    int deallocs = 0;
    for (int i = 0; i < allocs; i++)
    {
        allocator.deallocate(ptrs0[deallocs++], pagesToAllocate, em);
    }
    printString("Deallocations: ");
    printInt(deallocs);
    putc('\n');

    allocator.printBlockTable();
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


    allocator.printBlockTable();
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

    allocator.printBlockTable();
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
    allocator.printBlockTable();

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
    allocator.printBlockTable();
}