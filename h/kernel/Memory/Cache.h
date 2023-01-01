#ifndef PROJECT_SLAB_CACHE_H
#define PROJECT_SLAB_CACHE_H

#include "../../../lib/hw.h"
#include "./SlabImplementation.h"
#include "./BuddyAllocator.h"
#include "../Utils/Utils.h"

namespace kernel::memory
{
    class Cache;
} // namespace kernel::memory

void insertIntoCacheList(kernel::memory::Cache* cache);
bool isValidCahce(kernel::memory::Cache* cachep);


namespace kernel::memory
{
    class Slab;
    class Cache;

    using SlabCtorPtr = Slab * (*)(Cache*);
    using SlabDtorPtr = void (*)(Slab*);

    class Cache
    {
    public:
        friend void ::insertIntoCacheList(kernel::memory::Cache* cache);
        friend bool ::isValidCahce(kernel::memory::Cache* cachep);

        using FunPtr = void(*)(void*);
        // make static make function
        void* operator new(size_t);
        void operator delete(void*);
        Cache(const char* name, size_t size, FunPtr ctor, FunPtr dtor);

        void* allocate();
        void deallocate(void* ptr);
        size_t freeEmptySlabs();
        bool owns(void* ptr);
    private:

        Slab* findMinPartialSlab();
        Slab* findOwningSlab(void* ptr);
        void insertIntoList(Slab*& list_ptr, Slab* slab);
        void removeFromList(Slab*& list_ptr, Slab* slab);
        static size_t getListSize(Slab const* head);
    public:
        static constexpr size_t NAME_MAX_LENGTH = 32;
    public:
        const char* name();
        size_t objectSize();
        size_t totalSize();
        size_t slabCount();
        size_t objectsPerSlab();
        int totalUsage();
    private:
        friend class Slab;
        size_t allocationsSinceFree = 0;
        Cache* prev = nullptr;
        Cache* next = nullptr;
        const size_t obj_size;
        const FunPtr constructor;
        const FunPtr destructor;
        const SlabCtorPtr allocatePage;
        const SlabDtorPtr deallocatePage;
    public:
        static Cache kmem_cache_Cache;
    };
}
#endif