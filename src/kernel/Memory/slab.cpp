#include "../../../h/kernel/Memory/slab.h"
#include "../../../h/kernel/Memory/Cache.h"
#include "../../../h/kernel/Utils/Utils.h"
#include "../../../h/kernel/Memory/BuddyAllocator.h"

static kernel::memory::Cache* kmem_cache_cache = nullptr;
static kernel::memory::Cache* cache_list = nullptr;
static kmem_cache_t* buffer_caches[BLOCK_LISTS_SIZE];

bool isValidCahce(kernel::memory::Cache* cachep)
{
    auto cache = (kernel::memory::Cache*)cache_list;
    while (cache != nullptr)
    {
        if (cache == cachep) return true;
        cache = cache->next;
    }
    return false;
}

void insertIntoCacheList(kernel::memory::Cache* cache)
{
    if (cache == nullptr) return;
    if (cache_list != nullptr)
    {
        cache_list->prev = cache;
    }
    cache->next = cache_list;
    cache->prev = nullptr;
    cache_list = cache;
}

void insertIntoCacheList(kmem_cache_t* cachep)
{
    insertIntoCacheList((kernel::memory::Cache*)cachep);
}

void kmem_init(void* space, int block_num)
{
    /* Assign memory space to kernel allocator*/
    kernel::memory::BuddyAllocator::initialize(space, block_num);

    kmem_cache_cache = &kernel::memory::Cache::kmem_cache_Cache;
    /* Add the kmem_cache cache to the cache_list*/
    insertIntoCacheList(kmem_cache_cache);

    for (size_t i = 0; i < BLOCK_LISTS_SIZE; i++)
    {
        buffer_caches[i] = nullptr;
    }
}

kmem_cache_t* kmem_cache_create(
    const char* name, size_t size,
    void (*ctor)(void*), void (*dtor)(void*))
{
    if (kernel::utils::strlen(name) >
        kernel::memory::Cache::NAME_MAX_LENGTH)
    {
        // Set error code
        return nullptr;
    }

    if (size < sizeof(void*))
    {
        size = sizeof(void*);
    }

    if (size > 1 << 17)
    {
        // Set error code
        return nullptr;
    }

    if (dtor != nullptr && ctor == nullptr)
    {
        // Set error code
        return nullptr;
    }

    const auto cache =
        new kernel::memory::Cache(name, size, ctor, dtor);

    insertIntoCacheList(cache);

    return (kmem_cache_t*)cache;
}

int kmem_cache_shrink(kmem_cache_t* cachep)
{
    auto cache = (kernel::memory::Cache*)cachep;
    if (!isValidCahce(cache))
    {
        // Set error code
        return 0;
    }
    // Check if it's a valid cache
    return cache->freeEmptySlabs();
}

void* kmem_cache_alloc(kmem_cache_t* cachep)
{
    auto cache = (kernel::memory::Cache*)cachep;
    if (!isValidCahce(cache))
    {
        // Set error code
        return nullptr;
    }
    // Check if it's a valid cache
    return cache->allocate();
}

void kmem_cache_free(kmem_cache_t* cachep, void* objp)
{
    auto cache = (kernel::memory::Cache*)cachep;
    if (!isValidCahce(cache))
    {
        // Set error code
        return;
    }
    // Check if it's a valid cache
    cache->deallocate(objp);
}

void* kmalloc(size_t size)
{
    const auto size_index = kernel::utils::log2((size << 1) - 1);
    if (MIN_ORDER > size_index || size_index >= MAX_ORDER)
    {
        return nullptr;
    }

    if (buffer_caches[size_index] == nullptr)
    {
        char name_buffer[10] = "size-\0\0";
        if (size_index < 10)
        {
            name_buffer[5] = '0' + size_index;
        }
        else
        {
            name_buffer[5] = '0' + size_index / 10;
            name_buffer[6] = '0' + size_index % 10;
        }

        auto new_cache = kmem_cache_create(
            name_buffer,
            1 << size_index,
            nullptr, nullptr);

        if (new_cache == nullptr)
        {
            return nullptr;
        }

        buffer_caches[size_index] = new_cache;

        insertIntoCacheList(buffer_caches[size_index]);
    }
    return kmem_cache_alloc(buffer_caches[size_index]);
}

void kfree(const void* objp)
{
    if (objp == nullptr) return;
    for (size_t i = 0; i < BLOCK_LISTS_SIZE; i++)
    {
        if (buffer_caches[i] == nullptr) continue;
        auto cache = (kernel::memory::Cache*)buffer_caches[i];

        if (cache->owns(objp))
        {
            kmem_cache_free(buffer_caches[i], (void*)objp);
            return; // Set success code here
        }
    }
    // Set error code
}

void kmem_cache_destroy(kmem_cache_t* cachep)
{
    if (!isValidCahce((kernel::memory::Cache*)cachep))
    {
        // Set error code
        return;
    }


}