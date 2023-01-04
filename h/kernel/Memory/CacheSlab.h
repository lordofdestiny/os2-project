#ifndef PROJECT_SLABS_H
#define PROJECT_SLABS_H

#include "../../../lib/hw.h"
#include "./Cache.h"
#include "./BuddyAllocator.h"

namespace kernel::memory
{
    class Cache;
    class Slab;

    using SlabCtorPtr = Slab * (*)(Cache*);
    using SlabDtorPtr = void (*)(Slab*);

    class Slab
    {
        friend class Cache;
    private:
        static Slab* allocateSmallSlab(Cache* owner);
        static Slab* allocateLargeSlab(Cache* owner);
        void initialize(Cache* owner, char* buff, char* alloc);

        static void deallocateSmallSlab(Slab* slab);
        static void deallocateLargeSlab(Slab* slab);
        void destroyAll();
    public:
        static SlabCtorPtr getSlabAllocator(size_t obj_size);
        static SlabDtorPtr getSlabDeallocator(size_t obj_size);
        static unsigned int getSlabBlockOrder(size_t obj_size);
        static size_t getSlabCapacity(size_t obj_size, unsigned int block_order);
    public:
        Slab() = delete;
        Slab(Slab const&) = delete;
        Slab& operator=(Slab const&) = delete;
        ~Slab() = default;

        void* allocate();
        void deallocate(void* ptr);

        bool isFull() const { return freeSlotCount == m_capacity; };
        bool isEmpty() const { return freeSlotCount == 0; };
        size_t getFreeSlotCount() const { return freeSlotCount; };
        bool owns(void const* obj) const;
        size_t capacity() const { return m_capacity; }
    private:
        Slab* prev;
        Slab* next;
        Cache* owner;
        size_t m_capacity;
        size_t freeSlotCount;
        /*
         * Depending on owner->obj_size, pages can be:
         * 1. small (obj_size< PAGE_SIZE / 8)
         * 2. large (obj_size>= PAGE_SIZE / 8)
         *
         * In the first case, a whole page is allocated,
         * Slab header from this object is placed in front.
         * The rest of the page is used to store a vectorized representation
         * of a list "is_allocated", that keeps track of allocated object
         * and for the of data pointed to by the "buffer"* .
         *
         * In the second case, this object is allocated from size-N
         * buffer of appropriate size, and it then allocates a page
         * to store the objects into
         * "buffer" points to this page.
         */
        uint16 allocated_head = 0;
        uint16* is_allocated;
        char* buffer;
        static constexpr uint16 BUFFCTL_END = -1;
    };

} // namespace kernel::memory

#endif