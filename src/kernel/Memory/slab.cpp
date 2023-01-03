#include "../../../h/kernel/Memory/slab.h"
#include "../../../h/kernel/Memory/Cache.h"
#include "../../../h/kernel/Utils/Utils.h"
#include "../../../h/ConsoleUtils.h"
#include "../../../h/syscall_cpp.hpp"
#include "../../../h/kernel/Memory/MemoryErrorManager.h"
#include "../../../h/kernel/Memory/BuddyAllocator.h"

static kmem_cache_t* kmem_cache_cache = nullptr;
static kernel::memory::Cache* cache_list = nullptr;
static kmem_cache_t* buffer_caches[17 - 5 + 1];
static int last_api_error;

/* HELPER FUNCTIONS */

bool isValidCache(kernel::memory::Cache* cachep)
{
    if (cachep == nullptr) return false;
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

void removeFromCacheList(kernel::memory::Cache* cache)
{
    if (cache == nullptr || cache_list == nullptr) return;

    if (cache_list == cache)
    {
        cache_list = cache->next;
    }

    if (cache->next != nullptr)
    {
        cache->next->prev = cache->prev;
    }

    if (cache->prev != nullptr)
    {
        cache->prev->next = cache->next;
    }
}

/* API IMPLEMENTATION */
void kmem_init(void* space, int block_num)
{
    using namespace kernel::memory;
    /* Assign memory space to kernel allocator*/
    BuddyAllocator::initialize(space, block_num);

    Cache::initializeCacheCache();
    kmem_cache_cache = (kmem_cache_t*)&Cache::kmem_cache_Cache;
    /* Add the kmem_cache cache to the cache_list*/
    insertIntoCacheList(kmem_cache_cache);

    for (size_t i = 0; i < BUFFER_TYPE_COUNT; i++)
    {
        buffer_caches[i] = nullptr;
    }
}

kmem_cache_t* kmem_cache_create(
    const char* name, size_t size,
    void (*ctor)(void*), void (*dtor)(void*))
{
    last_api_error = 0;

    using kernel::memory::Cache;
    using kernel::utils::strlen;

    if (name == nullptr)
    {
        using namespace kernel::memory;
        last_api_error = MemoryErrorManager::
            getAPIErrorCode(APIError::NAME_UNDEFINED);
        return nullptr;
    }

    if (strlen(name) > Cache::NAME_MAX_LENGTH)
    {
        using namespace kernel::memory;
        last_api_error = MemoryErrorManager::
            getAPIErrorCode(APIError::NAME_TOO_LONG);
        return nullptr;
    }

    if (size > 1 << BUFFER_MAX_ORDER)
    {
        using namespace kernel::memory;
        last_api_error = MemoryErrorManager::
            getAPIErrorCode(APIError::OUT_OF_SIZE_RANGE);
        return nullptr;
    }

    if (dtor != nullptr && ctor == nullptr)
    {
        using namespace kernel::memory;
        last_api_error = MemoryErrorManager::
            getAPIErrorCode(APIError::CACHE_INVALID_CTOR);
        return nullptr;
    }

    const auto cache =
        new kernel::memory::Cache(name, size, ctor, dtor);

    insertIntoCacheList(cache);

    return (kmem_cache_t*)cache;
}

int kmem_cache_shrink(kmem_cache_t* cachep)
{
    last_api_error = 0;

    auto cache = (kernel::memory::Cache*)cachep;
    cache->getErrorManager().clear();

    if (!isValidCache(cache))
    {
        using namespace kernel::memory;
        last_api_error = MemoryErrorManager::
            getAPIErrorCode(APIError::NON_EXISTENT_CACHE);
        return 0;
    }
    return cache->freeEmptySlabs();
}

void* kmem_cache_alloc(kmem_cache_t* cachep)
{
    last_api_error = 0;

    auto cache = (kernel::memory::Cache*)cachep;
    cache->getErrorManager().clear();
    if (!isValidCache(cache))
    {
        using namespace kernel::memory;
        last_api_error = MemoryErrorManager::
            getAPIErrorCode(APIError::NON_EXISTENT_CACHE);
        return nullptr;
    }
    return cache->allocate();
}

void kmem_cache_free(kmem_cache_t* cachep, void* objp)
{
    last_api_error = 0;

    auto cache = (kernel::memory::Cache*)cachep;
    cache->getErrorManager().clear();
    if (!isValidCache(cache))
    {
        using namespace kernel::memory;
        last_api_error = MemoryErrorManager::
            getAPIErrorCode(APIError::NON_EXISTENT_CACHE);
        return;
    }
    cache->deallocate(objp);
}

void* kmalloc(size_t size)
{
    last_api_error = 0;

    const auto size_index = kernel::utils::log2((size << 1) - 1);
    if (BUFFER_MIN_ORDER > size_index
        || BUFFER_MAX_ORDER < size_index)
    {
        using namespace kernel::memory;
        last_api_error = MemoryErrorManager::
            getAPIErrorCode(APIError::INVALID_BUFFER_SIZE);
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
            using namespace kernel::memory;
            last_api_error = MemoryErrorManager::
                getAPIErrorCode(APIError::SIZE_N_ALLOC_FAILED);
            return nullptr;
        }

        buffer_caches[size_index] = new_cache;
    }
    return kmem_cache_alloc(buffer_caches[size_index]);
}

void kfree(const void* objp)
{
    last_api_error = 0;

    if (objp == nullptr)
    {
        using namespace kernel::memory;
        last_api_error = MemoryErrorManager::
            getAPIErrorCode(APIError::DEALLOCATE_NULLPTR);
        return;
    }
    for (size_t i = 0; i < BUFFER_TYPE_COUNT; i++)
    {
        if (buffer_caches[i] == nullptr) continue;
        auto cache = (kernel::memory::Cache*)buffer_caches[i];

        if (cache->owns(objp))
        {
            kmem_cache_free(buffer_caches[i], (void*)objp);
            return;
        }
    }
    using namespace kernel::memory;
    last_api_error = MemoryErrorManager::
        getAPIErrorCode(APIError::INVALID_DEALLOCATION_ADDRESS);
}

void kmem_cache_destroy(kmem_cache_t* cachep)
{
    last_api_error = 0;

    if (!isValidCache((kernel::memory::Cache*)cachep))
    {
        using namespace kernel::memory;
        last_api_error = MemoryErrorManager::
            getAPIErrorCode(APIError::NON_EXISTENT_CACHE);
        return;
    }


    auto cache = (kernel::memory::Cache*)cachep;
    cache->getErrorManager().clear();

    removeFromCacheList(cache);

    cache->destroyAllObjects();
    kmem_cache_free(kmem_cache_cache, cachep);
}

uint64 cache_info_lock = 0;
void kmem_cache_info(kmem_cache_t* cachep)
{
    if (!isValidCache((kernel::memory::Cache*)cachep))
    {
        using namespace kernel::memory;
        last_api_error = MemoryErrorManager::
            getAPIErrorCode(APIError::NON_EXISTENT_CACHE);
        return;
    }
    auto cache = (kernel::memory::Cache*)cachep;
    copy_and_swap(cache_info_lock, 0, 1);
    printString("NAME=");
    printString(cache->name());

    printString("; OBJ_SIZE=");
    printInt(cache->objectSize());

    printString("B; OBJECT/SLAB=");
    printInt(cache->objectsPerSlab());

    printString(" | TOTAL_SIZE=");
    printUInt64(cache->totalSize());

    printString("B; SLAB_CNT=");
    printInt(cache->slabCount());

    printString("; USAGE=");
    printInt(cache->totalUsage());
    printString("%\n");
    copy_and_swap(cache_info_lock, 1, 0);
}

int kmem_cache_error(kmem_cache_t* cachep)
{
    if (cachep == nullptr)
    {
        if (last_api_error == 0) return 0;
        const char* msg = kernel::memory::MemoryErrorManager::
            getAPIErrrorMessage(last_api_error);
        copy_and_swap(cache_info_lock, 0, 1);
        printString("errmsg: ");
        printString(msg);
        copy_and_swap(cache_info_lock, 1, 0);

        return last_api_error;
    }
    if (!isValidCache((kernel::memory::Cache*)cachep))
    {
        using namespace kernel::memory;
        last_api_error = MemoryErrorManager::
            getAPIErrorCode(APIError::NON_EXISTENT_CACHE);
        return -last_api_error;
    }
    auto cache = (kernel::memory::Cache*)cachep;
    if (not cache->getErrorManager().hasError())
    {
        return 0;
    }
    copy_and_swap(cache_info_lock, 0, 1);
    auto const& emng = cache->getErrorManager();
    printString("origin: ");
    printString(emng.getOrigin());
    printString(" | op: ");
    printString(emng.getOperation());
    printString(" | msg: ");
    printString(emng.getErrorMessage());
    putc('\n');
    copy_and_swap(cache_info_lock, 1, 0);

    return cache->getErrorManager().ecode();
}