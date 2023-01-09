#include "../../../h/kernel/Memory/CacheSlab.h"

namespace kernel::memory
{
    void* Slab::getElement(size_t index)
    {
        return
            const_cast<void*>
            (const_cast<const Slab*>
                (this)->getElement(index));
    }

    void const* Slab::getElement(size_t index) const
    {
        return buffer + index * owner->obj_size;
    }


    void Slab::initialize(Cache* owner, char* buff, char* alloc)
    {
        freelist = 0;
        prev = next = nullptr;
        this->owner = owner;
        m_capacity = owner->m_slabCapacity;
        freeSlotCount = m_capacity;
        buffer = buff;
        is_allocated = (uint16*)alloc;

        for (size_t i = 0; i < m_capacity - 1; i++)
        {
            is_allocated[i] = i + 1;
        }
        is_allocated[m_capacity - 1] = BUFFCTL_END;

        const auto constructor = owner->constructor;
        if (constructor == nullptr) return;

        for (size_t i = 0; i < m_capacity; i++)
        {
            constructor(getElement(i));
        }
    }

    /*
        if __name__ == "__main__":
            main()

    */

    Slab* Slab::allocateSmallSlab(Cache* owner)
    {
        auto slab = (Slab*)page_alloc(
            owner->slabBlockOrder,
            owner->errmng);
        /* No need for error logging here, it's already done in the allocate() */
        if (slab == nullptr) return nullptr;

        const auto obj_size = owner->obj_size;
        const auto capacity = owner->m_slabCapacity;

        const auto buffer_ptr = (char*)slab +
            (PAGE_SIZE - capacity * obj_size);
        const auto alloc_ptr = (char*)buffer_ptr
            - capacity * sizeof(uint16);

        slab->initialize(owner, buffer_ptr, alloc_ptr);

        return slab;
    }

    Slab* Slab::allocateLargeSlab(Cache* owner)
    {
        auto escope = owner->errmng
            .getScope(ErrorOrigin::CACHE, Operation::ALLOCATE);

        const auto capacity = owner->m_slabCapacity;

        const auto header_size = sizeof(Slab) + capacity * sizeof(uint16);

        auto slab = (Slab*)kmalloc(header_size);
        if (slab == nullptr)
        {
            escope.setError(SlabError::BIG_SLAB_HEADER_ALLOCATION_FAILED);
            return nullptr;
        }

        /* No need for error logging here, it's already done in the allocate() */
        const auto buffer = (char*)page_alloc(
            owner->slabBlockOrder,
            owner->errmng);

        if (buffer == nullptr)
        {
            kfree(slab);
            return nullptr;
        }

        const auto alloc_ptr = (char*)slab + header_size
            - capacity * sizeof(uint16);

        slab->initialize(owner, buffer, alloc_ptr);

        return slab;
    }

    void Slab::destroyAll()
    {
        const auto destructor = owner->destructor;
        if (destructor == nullptr) return;

        for (size_t i = 0; i < m_capacity; i++)
        {
            destructor(getElement(i));
        }

    }

    void Slab::deallocateSmallSlab(Slab* slab)
    {
        slab->destroyAll();
        page_free(slab,
            slab->owner->slabBlockOrder,
            slab->owner->errmng);
    }
    void Slab::deallocateLargeSlab(Slab* slab)
    {
        slab->destroyAll();

        page_free(
            slab->buffer,
            slab->owner->slabBlockOrder,
            slab->owner->errmng);
        kfree(slab);
    }

    SlabCtorPtr Slab::getSlabAllocator(size_t obj_size)
    {
        return obj_size < SMALL_SLAB_LIMIT
            ? allocateSmallSlab
            : allocateLargeSlab;

    }
    SlabDtorPtr Slab::getSlabDeallocator(size_t obj_size)
    {
        return obj_size < SMALL_SLAB_LIMIT
            ? Slab::deallocateSmallSlab
            : deallocateLargeSlab;
    }

    unsigned int Slab::getSlabBlockOrder(size_t obj_size)
    {
        return [](int a, int b)
        {
            return a > b ? a : b;
        }(utils::ceil_log2(obj_size) - PAGE_ORDER, 0);
    }

    size_t Slab::getSlabCapacity(size_t obj_size, unsigned int block_order)
    {
        return obj_size < SMALL_SLAB_LIMIT
            ? (PAGE_SIZE - sizeof(Slab)) / (obj_size + sizeof(uint16))
            : PAGE_SIZE * (1 << block_order) / obj_size;
    }

    void* Slab::allocate()
    {
        auto escope = owner->errmng
            .getScope(ErrorOrigin::SLAB, Operation::ALLOCATE);

        if (freelist == BUFFCTL_END)
        {
            escope.setError(SlabError::ALLOCATION_FAILED);
            return nullptr;
        }

        const auto obj = getElement(freelist);
        freelist = is_allocated[freelist];
        freeSlotCount--;
        return obj;
    }

    void Slab::deallocate(void* ptr)
    {
        auto escope = owner->errmng
            .getScope(ErrorOrigin::SLAB, Operation::DEALLOCATE);
        if (ptr == nullptr)
        {
            escope.setError(SlabError::DEALLOCATE_NULLPTR);
            return;
        }

        const auto obj_size = owner->obj_size;
        const auto index = ((char*)(ptr)-buffer + obj_size - 1) / obj_size;

        if (!owns(ptr) || ptr != getElement(index))
        {
            escope.setError(SlabError::INVALID_DEALLOCATION_ADDRESS);
            return;
        }

        is_allocated[index] = freelist;
        freelist = index;

        freeSlotCount++;
    }

    bool Slab::owns(void const* obj) const
    {
        return buffer <= obj &&
            obj < (char*)getElement(m_capacity);
    }

} // namespace kernem::memory
