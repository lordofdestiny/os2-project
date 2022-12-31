#include "../../../h/kernel/Memory/Cache.h"

namespace kernel::memory
{
    Cache::Cache(const char* name, size_t size, FunPtr ctor, FunPtr dtor)
        : obj_size(size), constructor(ctor), destructor(dtor),
        allocatePage(Slab::getSlabAllocator(size)),
        deallocatePage(Slab::getSlabDeallocator(size))
    {
        kernel::utils::strcpy(this->m_name, name);
    }


    void* Cache::allocate()
    {
        // Check if there is a free slot in any of the partial slabs
        auto slab = findMinPartialSlab();
        // If found allocate from it, if not, allocate a free slab and use it
        if (slab == nullptr)
        {
            const auto new_slab = allocatePage(this);
            // One slot will be allocated from this slab, so it can be placed into
            // the partial list immediately
            insertIntoList(partial, new_slab);
            slab = new_slab;
        }
        const auto obj = slab->allocate();

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
        // Find which slab from partial list this ptr bellongs to
        auto slab = findOwningSlab(ptr);
        if (slab == nullptr) return;

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
            deallocatePage(slab);
            cnt++;
        }

        allocationsSinceFree = 0;
        return cnt;
    }

    bool Cache::owns(void* ptr)
    {
        return false;
    }

    Slab* Cache::findMinPartialSlab()
    {
        if (partial == nullptr) return nullptr;

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

    Slab* Cache::findOwningSlab(void* ptr)
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

    const char* Cache::name()
    {
        return m_name;
    }
    size_t Cache::objectSize()
    {
        return obj_size;
    }
    size_t Cache::slabCount()
    {
        return getListSize(free)
            + getListSize(partial)
            + getListSize(full);
    }
    size_t Cache::totalSize()
    {
        return PAGE_SIZE * slabCount();
    }
    size_t Cache::objectsPerSlab()
    {
        return (PAGE_SIZE - sizeof(Slab))
            / (obj_size + sizeof(bool));
    }
    int Cache::totalUsage()
    {
        auto totalCount = slabCount() * objectsPerSlab();
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
} // namespace kernem::memory