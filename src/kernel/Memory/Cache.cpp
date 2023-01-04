#include "../../../h/kernel/Memory/Cache.h"

namespace kernel::memory
{
    Cache Cache::obj_cache;
    bool Cache::initialized = false;

    void Cache::initializeCacheCache()
    {
        if (!initialized)
        {
            const auto obj_size = sizeof(Cache);
            const auto blockOrder = Slab::getSlabBlockOrder(obj_size);
            const auto blockCap = Slab::getSlabCapacity(obj_size, blockOrder);

            obj_cache.obj_size = obj_size;
            obj_cache.allocateSlab = Slab::getSlabAllocator(obj_size);
            obj_cache.deallocateSlab = Slab::getSlabDeallocator(obj_size);
            obj_cache.slabBlockOrder = blockOrder;
            obj_cache.slabCapacity = blockCap;

            utils::strcpy(obj_cache.m_name, "kmem_cache_t");

            initialized = true;
        }
    }

    void* Cache:: operator new(size_t)
    {
        return obj_cache.allocate();
    }

    void Cache:: operator delete(void* ptr)
    {
        return obj_cache.deallocate(ptr);
    }

    Cache::Cache(const char* name, size_t size, FunPtr ctor, FunPtr dtor)
        : obj_size(size), constructor(ctor), destructor(dtor),
        allocateSlab(Slab::getSlabAllocator(size)),
        deallocateSlab(Slab::getSlabDeallocator(size)),
        slabBlockOrder(Slab::getSlabBlockOrder(size)),
        slabCapacity(Slab::getSlabCapacity(size, slabBlockOrder))
    {
        kernel::utils::strcpy(this->m_name, name);
    }

    void Cache::deallocateList(Slab*& list)
    {
        while (list != nullptr)
        {
            auto tmp = list;
            removeFromList(list, tmp);
            deallocateSlab(tmp);
        }
    }

    void Cache::destroyAllObjects()
    {
        deallocateList(free);
        deallocateList(partial);
        deallocateList(full);
    }


    void* Cache::allocate()
    {
        // Register due to consistency, but no error is ever reported here
        auto scope = errmng.getScope(ErrorOrigin::CACHE, Operation::ALLOCATE);

        // Check if there is a free slot in any of the partial slabs
        auto slab = findMinNonEmptySlab();

        // If found allocate from it, if not, allocate a free slab and use it
        if (slab == nullptr)
        {
            // Error already reported in the buddy allocator
            const auto new_slab = allocateSlab(this);
            if (new_slab == nullptr) return nullptr;
            // One slot will be allocated from this slab, so it can be placed into
            // the partial list immediately

            insertIntoList(partial, new_slab);

            slab = new_slab;
        }
        // Error already reported in the slab
        const auto obj = slab->allocate();
        if (obj == nullptr) return nullptr;

        // If a slab that was allocated from was completly used,
        // place it into the full list

        if (slab->isEmpty())
        {
            removeFromList(partial, slab);
            insertIntoList(full, slab);
        }

        return obj;
    }

    void Cache::deallocate(void* ptr)
    {
        auto escope = errmng.getScope(ErrorOrigin::CACHE, Operation::DEALLOCATE);
        if (ptr == nullptr)
        {
            escope.setError(CacheError::DEALLOCATE_NULLPTR);
            return;
        }
        // Find which slab from partial list this ptr bellongs to
        auto slab = findOwningSlab(ptr);
        if (slab == nullptr)
        {
            escope.setError(CacheError::INVALID_DEALLOCATION_ADDRESS);
            return;
        }

        // If salb was previously fully allocated
        if (slab->isEmpty())
        {
            removeFromList(full, slab);
            insertIntoList(partial, slab);
        }

        // Return it to the slab
        slab->deallocate(ptr);

        // If slab is now full, move it to free list
        if (slab->isFull())
        {
            removeFromList(partial, slab);
            insertIntoList(free, slab);
        }
    }

    size_t Cache::freeEmptySlabs()
    {
        if (allocationsSinceFree > 0)
        {
            allocationsSinceFree = 0;
            return 0;
        }

        size_t cnt = 0;
        while (free != nullptr)
        {
            auto slab = free;
            removeFromList(free, slab);
            deallocateSlab(slab);
            cnt++;
        }

        allocationsSinceFree = 0;
        return cnt;
    }

    bool Cache::owns(void const* ptr) const
    {
        return findOwningSlab(ptr) != nullptr;
    }

    Slab* Cache::findMinNonEmptySlab()
    {
        if (partial == nullptr)
        {
            if (free == nullptr) return nullptr;
            /*
             * If there is  parital slabs
             * try to allocate from first free slab
             */
            auto slab = free;
            removeFromList(free, slab);
            insertIntoList(partial, slab);
            return slab; // return first free slab
        }
        auto min_slab = partial;
        auto min = min_slab->capacity();

        auto slab = partial;
        while (slab != nullptr)
        {
            if (slab->getFreeSlotCount() < min)
            {
                min_slab = slab;
                min = min_slab->getFreeSlotCount();
            }
            slab = slab->next;
        }

        return min_slab;
    }

    Slab* Cache::findOwningSlab(void const* ptr) const
    {
        auto slab = partial;
        while (slab != nullptr)
        {
            if (slab->owns(ptr)) return slab;
            slab = slab->next;
        }
        slab = full;
        while (slab != nullptr)
        {
            if (slab->owns(ptr)) return slab;
            slab = slab->next;
        }

        return nullptr;
    }

    void Cache::insertIntoList(Slab*& list_ptr, Slab* slab)
    {
        if (slab == nullptr) return;
        if (list_ptr != nullptr)
        {
            list_ptr->prev = slab;
        }
        slab->next = list_ptr;
        slab->prev = nullptr;
        list_ptr = slab;
    }

    void Cache::removeFromList(Slab*& list_ptr, Slab* slab)
    {
        if (slab == nullptr || list_ptr == nullptr) return;

        if (list_ptr == slab)
        {
            list_ptr = slab->next;
        }

        if (slab->next != nullptr)
        {
            slab->next->prev = slab->prev;
        }

        if (slab->prev != nullptr)
        {
            slab->prev->next = slab->next;
        }
    }

    size_t Cache::getListSize(Slab const* head)
    {
        size_t size = 0;
        while (head != nullptr)
        {
            size++;
            head = head->next;
        }
        return size;
    }

    const char* Cache::name() const
    {
        return m_name;
    }
    size_t Cache::objectSize() const
    {
        return obj_size;
    }
    size_t Cache::slabCount() const
    {
        return getListSize(free)
            + getListSize(partial)
            + getListSize(full);
    }
    size_t Cache::totalSize() const
    {
        return PAGE_SIZE * (1 << slabBlockOrder) * slabCount();
    }
    size_t Cache::objectsPerSlab() const
    {
        return obj_size < (PAGE_SIZE >> 3)
            ? (PAGE_SIZE - sizeof(Slab))
            / (obj_size + sizeof(bool))
            : (PAGE_SIZE + obj_size - 1) / obj_size;
    }
    int Cache::totalUsage() const
    {
        const auto totalCount = slabCount() * objectsPerSlab();
        if (totalCount == 0) return 0;

        auto freeCount = getListSize(free) * objectsPerSlab();
        auto slab = partial;
        while (slab != 0)
        {
            freeCount += slab->freeSlotCount;
            slab = slab->next;
        }

        return (totalCount - freeCount) * 100 / totalCount;
    }

    MemoryErrorManager&
        Cache::getErrorManager()
    {
        return errmng;
    }
    MemoryErrorManager const&
        Cache::getErrorManager() const
    {
        return errmng;
    }
} // namespace kernem::memory