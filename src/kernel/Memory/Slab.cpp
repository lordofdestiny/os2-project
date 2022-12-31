#include "../../../h/kernel/Memory/Slab.h"

namespace kernel::memory
{
    Slab* Slab::allocateSlab(small_slab_tag_t, Cache* owner)
    {
        auto slab = (Slab*)BuddyAllocator::getInstance().allocate(1);
        if (slab == nullptr) return nullptr;

        slab->prev = nullptr;
        slab->next = nullptr;
        slab->owner = owner;

        const auto capacity = calculateCapacity(owner);
        slab->m_capacity = capacity;
        slab->freeSlotCount = capacity;

        slab->buffer = calculateBufferAddress(slab);
        slab->allocated = calulateAllocatedAddress(slab);
        slab->initialize();

        return slab;
    }

    Slab* Slab::allocateSlab(large_slab_tag_t, Cache* owner)
    {
        /* NOT IMPLEMENTED*/
        return nullptr;
    }

    void Slab::deallocateSlab(small_slab_tag_t, Slab* slab)
    {
        const auto destructor = slab->owner->destructor;
        const auto obj_size = slab->owner->obj_size;
        const auto cap = slab->m_capacity;
        const auto buff = slab->buffer;
        if (destructor != nullptr)
        {

            for (size_t i = 0; i < cap; i++)
            {
                destructor(buff + i * obj_size);
            }
        }
    }
    void Slab::deallocateSlab(large_slab_tag_t, Slab* slab)
    {
        /* NOT IMPLEMENTED*/
    }

    SlabCtorPtr Slab::getSlabAllocator(size_t obj_size)
    {
        if (obj_size < PAGE_SIZE >> 3)
        {
            return (SlabCtorPtr)[](Cache* owner) {
                return Slab::allocateSlab(small_slab_tag_t{}, owner);
            };
        }
        else
        {
            return (SlabCtorPtr)[](Cache* owner) {
                return Slab::allocateSlab(large_slab_tag_t{}, owner);
            };
        }
    }
    SlabDtorPtr Slab::getSlabDeallocator(size_t obj_size)
    {
        if (obj_size < PAGE_SIZE >> 3)
        {
            return (SlabDtorPtr)[](Slab* slab) {
                return Slab::deallocateSlab(small_slab_tag_t{}, slab);
            };
        }
        else
        {
            return (SlabDtorPtr)[](Slab* slab) {
                return Slab::deallocateSlab(large_slab_tag_t{}, slab);
            };
        }
    }

    void Slab::initialize()
    {
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

    size_t Slab::calculateCapacity(Cache* owner)
    {
        return (PAGE_SIZE - sizeof(Slab))
            / (owner->obj_size + sizeof(bool));
    }

    char* Slab::calculateBufferAddress(Slab* slab)
    {
        return (char*)slab
            + (PAGE_SIZE - slab->m_capacity * slab->owner->obj_size);
    }

    bool* Slab::calulateAllocatedAddress(Slab* slab)
    {
        return (bool*)((char*)
            slab->buffer
            - sizeof(bool) * slab->owner->obj_size);
    }

    void* Slab::allocate()
    {
        const auto obj_size = owner->obj_size;
        for (size_t i = 0; i < m_capacity; i++)
        {
            if (!allocated[i])
            {
                allocated[i] = true;
                freeSlotCount--;
                return buffer + i * obj_size;
            }
        }

        return nullptr;
    }

    void Slab::deallocate(void* ptr)
    {
        if (!owns(ptr)) return;
        const auto obj_size = owner->obj_size;
        auto index = ((uint64)(ptr)-(uint64)buffer + obj_size - 1) / obj_size;
        if (ptr != (void*)(buffer + index * obj_size)) return;
        allocated[index] = false;
        freeSlotCount++;
    }

    bool Slab::isFull() const
    {
        return freeSlotCount == m_capacity;
    }

    bool Slab::isEmpty() const
    {
        return freeSlotCount == 0;
    }

    size_t Slab::getFreeSlotCount() const
    {
        return freeSlotCount;
    }

    bool Slab::owns(void* obj) const
    {
        return (uint64)buffer <= (uint64)obj
            && (uint64)(obj) < (uint64)((char*)buffer + PAGE_SIZE);
    }

    size_t Slab::capacity() const
    {
        return m_capacity;
    }
} // namespace kernem::memory
