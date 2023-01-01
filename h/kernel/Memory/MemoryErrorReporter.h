#ifndef PROJECT_MEMORY_ERROR_REPORTER_H
#define PROJECT_MEMORY_ERROR_REPORTER_H

namespace kernel::memory
{
    enum class ErrorOrigin
    {
        BUDDY = 0x01,
        SLAB_IMPL = 0x02,
        CACHE = 0x03,
        API = 0x04
    };

    enum class BuddyError
    {
        INVALID_PAGE_ORDER = (int)ErrorOrigin::BUDDY + 1,
        ALLOCATION_FAILED,
        DEALLOCATE_NULLPTR,
        INVALID_DEALLOCATION_ADDRESS
    };

    enum class SlabImplError
    {
        SLAB_FULL_ERROR = (int)ErrorOrigin::SLAB_IMPL + 1,
        DEALLOCATE_NULLPTR,
        INVALID_DEALLOCATION_ADDRESS,
    };

    enum class CacheError
    {
        CACHE_EXTENDING_FAILED = (int)ErrorOrigin::CACHE + 1,
        DEALLOCATE_NULLPTR,
        INVALID_DEALLOCATION_ADDRESS,
    };

    enum class APIError
    {
        OUT_OF_SIZE_RANGE = (int)ErrorOrigin::API + 1,
        DEALLOCATE_NULLPTR,
        NON_EXISTENT_CACHE
    };
}

#endif
