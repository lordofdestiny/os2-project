#ifndef PROJECT_BUDDY_LLOCATOR_H
#define PROJECT_BUDDY_LLOCATOR_H

#define PAGE_ORDER 12
#define PAGE_SIZE (1 << PAGE_ORDER)

#include "../../../lib/hw.h"
#include  "./MemoryErrorManager.h"
#include "../Kernel.h"

/* Implementation is based on the following link :
 * https://github.com/red-rocket-computing/buddy-alloc/blob/master/doc/bitsquid-buddy-allocator-design.md
*/

namespace kernel::memory
{
    void buddy_init(void* addr, uint64 blocks);
    void* page_alloc(int order);
    void page_free(void* addr, int order);
    void* page_alloc(int order, ErrorManager& emg);
    void page_free(void* addr, int order, ErrorManager& emg);
    void printBlockTable();
    void printBlockTable();

    struct Stats
    {
        size_t aloocated;
        size_t total;
    };

    Stats getStats();
}

#endif