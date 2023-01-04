#ifndef PROJECT_SLAB_CACHE_H
#define PROJECT_SLAB_CACHE_H

#include "../../../lib/hw.h"
#include "./CacheSlab.h"
#include "./BuddyAllocator.h"
#include "../Utils/Utils.h"
#include "./MemoryErrorManager.h"
#include "../Kernel.h"
#include "../../../h/syscall_c.h"

#define M_NAME_MAX_LENGTH 31
#define BUFFER_MIN_ORDER 5
#define BUFFER_MAX_ORDER 17
#define BUFFER_TYPE_COUNT (BUFFER_MAX_ORDER-BUFFER_MIN_ORDER+1)

namespace kernel
{
    class Kernel;
} // namespace kernel


namespace kernel::memory
{
    class Cache;
} // namespace kernel::memory

void insertIntoCacheList(kernel::memory::Cache* cache);
bool isValidCache(kernel::memory::Cache* cachep);
void removeFromCacheList(kernel::memory::Cache* cache);
void kmem_init(void*, int);

namespace kernel::memory
{
    class Slab;
    class Cache;

    using SlabCtorPtr = Slab * (*)(Cache*);
    using SlabDtorPtr = void (*)(Slab*);

    class Cache
    {
    public:
        friend class kernel::Kernel;
        friend void ::insertIntoCacheList(kernel::memory::Cache* cache);
        friend bool ::isValidCache(kernel::memory::Cache* cachep);
        friend void ::removeFromCacheList(kernel::memory::Cache* cache);

        using FunPtr = void(*)(void*);
        // make static make function
        void* operator new(size_t);
        void operator delete(void*);

    private:
        Cache() = default;
    public:
        Cache(const char* name, size_t size, FunPtr ctor, FunPtr dtor);
        Cache(Cache const&) = delete;
        Cache& operator=(Cache const&) = delete;
        ~Cache() = default;

        void* allocate();
        void deallocate(void* ptr);
        size_t freeEmptySlabs();
        bool owns(void const* ptr) const;
    public:
        void destroyAllObjects();
    private:
        void deallocateList(Slab*& list);
        Slab* findMinNonEmptySlab();
        Slab* findOwningSlab(void const* ptr) const;
        void insertIntoList(Slab*& list_ptr, Slab* slab);
        void removeFromList(Slab*& list_ptr, Slab* slab);
        static size_t getListSize(Slab const* head);
    public:
        static constexpr size_t NAME_MAX_LENGTH = M_NAME_MAX_LENGTH;

        const char* name() const;
        size_t objectSize() const;
        size_t totalSize() const;
        size_t slabCount() const;
        size_t objectsPerSlab() const;
        int totalUsage() const;
        MemoryErrorManager& getErrorManager();
        MemoryErrorManager const& getErrorManager() const;
    private:
        friend class Slab;
        size_t allocationsSinceFree = 0;

        Slab* free = nullptr;
        Slab* partial = nullptr;
        Slab* full = nullptr;

        Cache* prev = nullptr;
        Cache* next = nullptr;

        size_t obj_size;

        FunPtr constructor = nullptr;
        FunPtr destructor = nullptr;

        SlabCtorPtr allocateSlab;
        SlabDtorPtr deallocateSlab;
        unsigned int slabBlockOrder;
        size_t slabCapacity;

        MemoryErrorManager errmng{};

        char m_name[NAME_MAX_LENGTH + 1];

        friend void ::kmem_init(void*, int);
        static Cache obj_cache;
        static bool initialized;
    public:
        static void initializeCacheCache();
    };
}
#endif