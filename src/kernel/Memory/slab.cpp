#include "../../../h/kernel/Memory/slab.h"
#include "../../../h/kernel/Utils/Utils.h"
#include "../../../h/ConsoleUtils.h"
#include "../../../h/syscall_c.h"
#include "../../../h/kernel/Memory/MemoryErrorManager.h"
#include "../../../h/kernel/Memory/BuddyAllocator.h"


#define FAIL_VOID(COND, ERROR) \
do{ \
if(COND) \
{\
    using namespace kernel::memory; \
    last_api_error = MemoryErrorManager::getAPIErrorCode(ERROR); \
    return; \
}}while(0)\

#define VALID_CACHE_VOID(cache) \
    FAIL_VOID(!kernel::Kernel::isValidCache(cache),\
    APIError::NON_EXISTENT_CACHE);\


#define FAIL_VAL(COND, ERROR, RETVAL) \
do{ \
if(COND) \
{\
    using namespace kernel::memory; \
    last_api_error = MemoryErrorManager\
        ::getAPIErrorCode(ERROR); \
    return RETVAL; \
}}while(0)\

#define VALID_CACHE_VAL(cache, RETVAL) \
    FAIL_VAL(!kernel::Kernel::isValidCache(cache),\
    APIError::NON_EXISTENT_CACHE,\
    RETVAL);\


#define AS_CACHE(ptr) ((kernel::memory::Cache*)ptr)


#define SHRINK(cache) \
do{\
if (AS_CACHE(cachep)->usage() > 70)\
{\
    kmem_cache_shrink(cachep);\
}}while(0)\


static kmem_cache_t* kmem_cache_cache = nullptr;
static kmem_cache_t* buffer_caches[
    BUFFER_MAX_ORDER - BUFFER_MIN_ORDER + 1
];
static int last_api_error = 0;

/* HELPER FUNCTIONS */

/* API IMPLEMENTATION */
void kmem_init(void* space, int block_num)
{
    using namespace kernel;
    using namespace memory;
    /* Assign memory space to kernel allocator*/
    BLOCKS.initialize(space, block_num);

    Cache::obj_cache.initialize();
    kmem_cache_cache = (kmem_cache_t*)&Cache::obj_cache;
    /* Add the kmem_cache cache to the cache_list*/
    Kernel::insertIntoCacheList(kmem_cache_cache);

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

    FAIL_VAL(
        name == nullptr,
        APIError::NAME_UNDEFINED, nullptr);

    FAIL_VAL(
        strlen(name) > Cache::NAME_MAX_LENGTH,
        APIError::NAME_TOO_LONG, nullptr);

    FAIL_VAL(
        size > 1 << BUFFER_MAX_ORDER,
        APIError::OUT_OF_SIZE_RANGE, nullptr);

    FAIL_VAL(
        dtor != nullptr && ctor == nullptr,
        APIError::CACHE_INVALID_CTOR, nullptr);

    const auto cache =
        new kernel::memory::Cache(name, size, ctor, dtor);

    kernel::Kernel::insertIntoCacheList(cache);

    return (kmem_cache_t*)cache;
}

int kmem_cache_shrink(kmem_cache_t* cachep)
{
    last_api_error = 0;
    VALID_CACHE_VAL(cachep, 0);
    AS_CACHE(cachep)->getErrorManager().clear();

    return AS_CACHE(cachep)->freeEmptySlabs();
}

void* kmem_cache_alloc(kmem_cache_t* cachep)
{
    last_api_error = 0;
    VALID_CACHE_VAL(cachep, nullptr);
    AS_CACHE(cachep)->getErrorManager().clear();
    SHRINK(cachep);

    return AS_CACHE(cachep)->allocate();
}

void kmem_cache_free(kmem_cache_t* cachep, void* objp)
{
    last_api_error = 0;
    VALID_CACHE_VOID(cachep);
    AS_CACHE(cachep)->getErrorManager().clear();

    AS_CACHE(cachep)->deallocate(objp);

    SHRINK(cachep);
}

void* kmalloc(size_t size)
{
    last_api_error = 0;

    const auto size_order = kernel::utils::ceil_log2(size);

    FAIL_VAL(
        BUFFER_MIN_ORDER > size_order
        || BUFFER_MAX_ORDER < size_order,
        APIError::INVALID_BUFFER_SIZE, nullptr);

    const auto size_index = size_order - BUFFER_MIN_ORDER;

    if (buffer_caches[size_index] == nullptr)
    {
        char name_buffer[10] = "size-\0\0";
        if (size_order < PAGE_ORDER - 3)
        {
            name_buffer[5] = '0' + size_order;
        }
        else
        {
            name_buffer[5] = '0' + size_order / 10;
            name_buffer[6] = '0' + size_order % 10;
        }

        const auto new_cache =
            kmem_cache_create(
                name_buffer,
                1 << size_order,
                nullptr, nullptr);

        FAIL_VAL(
            new_cache == nullptr,
            APIError::SIZE_N_ALLOC_FAILED, nullptr);

        buffer_caches[size_index] = new_cache;
    }
    return kmem_cache_alloc(buffer_caches[size_index]);
}

void kfree(const void* objp)
{
    last_api_error = 0;

    FAIL_VOID(objp == nullptr, APIError::DEALLOCATE_NULLPTR);

    for (size_t i = 0; i < BUFFER_TYPE_COUNT; i++)
    {
        if (buffer_caches[i] == nullptr) continue;
        if (!AS_CACHE(buffer_caches[i])->owns(objp)) continue;

        kmem_cache_free(buffer_caches[i], (void*)objp);
        return;
    }
    FAIL_VOID(true, APIError::INVALID_DEALLOCATION_ADDRESS);
}

void kmem_cache_destroy(kmem_cache_t* cachep)
{
    last_api_error = 0;

    VALID_CACHE_VOID(cachep);

    auto cache = (kernel::memory::Cache*)cachep;
    cache->getErrorManager().clear();

    kernel::Kernel::removeFromCacheList(cache);

    cache->destroyAllObjects();
    kmem_cache_free(kmem_cache_cache, cachep);
}

uint64 cache_info_lock = 0;
void kmem_cache_info(kmem_cache_t* cachep)
{
    VALID_CACHE_VOID(cachep);

    auto cache = (kernel::memory::Cache*)cachep;
    copy_and_swap(cache_info_lock, 0, 1);
    printString("NAME=");
    printString(cache->name());

    printString("; OBJ_SIZE=");
    printInt(cache->objectSize());

    printString("B; OBJECT/SLAB=");
    printInt(cache->slabCapacity());

    printString(" | TOTAL_SIZE=");
    printUInt64(cache->memory() / BLOCK_SIZE);

    printString("B; SLAB_CNT=");
    printInt(cache->slabs());

    const auto totalSlots = cache->capacity();
    const auto freeSlots = cache->available();
    const auto usedSlots = totalSlots - freeSlots;
    printString("; USAGE= ");
    printInt(usedSlots);
    printString("/");
    printInt(cache->capacity());
    printString(" ( ");
    printInt(cache->usage());
    printString("% )\n");
    copy_and_swap(cache_info_lock, 1, 0);
}

int kmem_cache_error(kmem_cache_t* cachep)
{
    if (cachep == nullptr && last_api_error == 0) return 0;
    if (last_api_error != 0)
    {
        const char* msg = kernel::memory::MemoryErrorManager::
            getAPIErrorMessage(last_api_error);
        copy_and_swap(cache_info_lock, 0, 1);
        printString("errmsg: ");
        printString(msg);
        copy_and_swap(cache_info_lock, 1, 0);

        return last_api_error;
    }

    VALID_CACHE_VAL(cachep, -last_api_error);

    if (!AS_CACHE(cachep)->getErrorManager().hasError()) return 0;

    copy_and_swap(cache_info_lock, 0, 1);
    auto const& emng = AS_CACHE(cachep)->getErrorManager();
    printString("origin: ");
    printString(emng.getOrigin());
    printString(" | op: ");
    printString(emng.getOperation());
    printString(" | msg: ");
    printString(emng.getErrorMessage());
    putc('\n');
    copy_and_swap(cache_info_lock, 1, 0);

    return AS_CACHE(cachep)->getErrorManager().ecode();
}