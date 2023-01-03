#include "../../../h/kernel/Memory/CacheSlab.h"
#include "../../../h/kernel/Memory/slab.h"

namespace kernel::memory
{
    void Slab::initialize(Cache* owner, char* buff, char* alloc)
    {
        prev = nullptr;
        next = nullptr;
        this->owner = owner;
        m_capacity = owner->slabCapacity;
        freeSlotCount = m_capacity;
        buffer = buff;
        allocated = (bool*)alloc;

        for (size_t i = 0; i < m_capacity; i++)
        {
            allocated[i] = false;
        }

        const auto constructor = owner->constructor;
        const auto obj_size = owner->obj_size;
        if (constructor == nullptr) return;

        for (size_t i = 0; i < m_capacity; i++)
        {
            constructor(buffer + i * obj_size);
        }
    }

    Slab* Slab::allocateSmallSlab(Cache* owner)
    {
        auto slab = (Slab*)BuddyAllocator::getInstance()
            .allocate(0, owner->errmng);
        /* No need for error logging here, it's already done in the allocate() */
        if (slab == nullptr) return nullptr;

        const auto obj_size = owner->obj_size;
        const auto capacity = owner->slabCapacity;

        const auto buffer_ptr = (char*)slab +
            (PAGE_SIZE - capacity * obj_size);
        const auto alloc_ptr = (char*)buffer_ptr
            - capacity * sizeof(bool);

        slab->initialize(owner, buffer_ptr, alloc_ptr);

        return slab;
    }

    Slab* Slab::allocateLargeSlab(Cache* owner)
    {
        auto escope = owner->errmng
            .getScope(ErrorOrigin::CACHE, Operation::ALLOCATE);

        const auto capacity = owner->slabCapacity;

        const auto header_size = sizeof(Slab) + capacity * sizeof(bool);

        auto slab = (Slab*)kmalloc(header_size);
        if (slab == nullptr)
        {
            escope.setError(SlabError::BIG_SLAB_HEADER_ALLOCATION_FAILED);
            return nullptr;
        }

        /* No need for error logging here, it's already done in the allocate() */
        const auto buffer = (char*)BuddyAllocator::getInstance()
            .allocate(owner->slabBlockOrder, owner->errmng);

        if (buffer == nullptr)
        {
            kfree(slab);
            return nullptr;
        }

        const auto alloc_ptr = (char*)slab + header_size
            - capacity * sizeof(bool);

        slab->initialize(owner, buffer, alloc_ptr);

        return slab;
    }

    void Slab::destroyAll()
    {
        const auto destructor = owner->destructor;
        const auto obj_size = owner->obj_size;
        if (destructor != nullptr)
        {
            for (size_t i = 0; i < m_capacity; i++)
            {
                destructor(buffer + i * obj_size);
            }
        }
    }

    void Slab::deallocateSmallSlab(Slab* slab)
    {
        slab->destroyAll();
        BuddyAllocator::getInstance()
            .deallocate(slab, 0, slab->owner->errmng);
    }
    void Slab::deallocateLargeSlab(Slab* slab)
    {
        slab->destroyAll();

        BuddyAllocator::getInstance()
            .deallocate(
                slab->buffer,
                slab->owner->slabBlockOrder,
                slab->owner->errmng);

        kfree(slab);
    }

    SlabCtorPtr Slab::getSlabAllocator(size_t obj_size)
    {
        return obj_size < PAGE_SIZE >> 3
            ? allocateSmallSlab
            : allocateLargeSlab;

    }
    SlabDtorPtr Slab::getSlabDeallocator(size_t obj_size)
    {
        return obj_size < PAGE_SIZE >> 3
            ? Slab::deallocateSmallSlab
            : deallocateLargeSlab;
    }

    unsigned int Slab::getSlabBlockOrder(size_t obj_size)
    {
        using fptr = int(*)(int, int);
        static fptr max = [](int a, int b) { return a > b ? a : b; };
        const auto buffer_order = utils::log2((obj_size << 1) - 1); // FIX
        return max(buffer_order - PAGE_ORDER, 0);
    }

    size_t Slab::getSlabCapacity(size_t obj_size, unsigned int block_order)
    {
        return obj_size < PAGE_SIZE >> 3
            ? (PAGE_SIZE - sizeof(Slab)) / (obj_size + sizeof(bool))
            : PAGE_SIZE * (1 << block_order) / obj_size;
    }

    void* Slab::allocate()
    {
        auto escope = owner->errmng
            .getScope(ErrorOrigin::SLAB, Operation::ALLOCATE);
        const auto obj_size = owner->obj_size;

        for (size_t i = 0; i < m_capacity; i++)
        {
            if (allocated[i]) continue;

            allocated[i] = true;
            freeSlotCount--;
            return buffer + i * obj_size;
        }
        escope.setError(SlabError::ALLOCATION_FAILED);
        return nullptr;
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
        const auto index =
            ((uint64)(ptr)-(uint64)buffer + obj_size - 1) / obj_size;

        if (!owns(ptr) || ptr != (void*)(buffer + index * obj_size))
        {
            escope.setError(SlabError::INVALID_DEALLOCATION_ADDRESS);
            return;
        }

        if (!allocated[index])
        {
            escope.setError(SlabError::UNALLOCATED_OBJECT_FREE);
            return;
        }

        allocated[index] = false;
        freeSlotCount++;
    }

    bool Slab::owns(void const* obj) const
    {
        return buffer <= obj && obj < buffer + PAGE_SIZE;
    }

} // namespace kernem::memory
