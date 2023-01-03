#include "../../../h/kernel/Memory/MemoryErrorManager.h"
#include "../../../h/kernel/Memory/Cache.h"

/*
 * Turn A into a string literal without expanding macro definitions
 */
#define STRINGIZE_NX(A) #A
 /*
  * Turn A into a string literal after macro-expanding it.
  */
#define STRINGIZE(A) STRINGIZE_NX(A)


namespace kernel::memory
{
    auto MemoryErrorManager::getScope(ErrorOrigin eo, Operation op)
        ->ErrorScope
    {
        return { this, eo, op };
    }
    void MemoryErrorManager::ErrorScope::setError(BuddyError be) { cause |= (int)be; }
    void MemoryErrorManager::ErrorScope::setError(SlabError se) { cause |= (int)se; }
    void MemoryErrorManager::ErrorScope::setError(CacheError ce) { cause |= (int)ce; }
    void MemoryErrorManager::clear() { errorCode = 0; };
    bool MemoryErrorManager::hasError() const { return errorCode != 0; }
    const char* MemoryErrorManager::getOperation()const
    {
        auto org = (ErrorOrigin)(errorCode & ORIGIN_MASK);
        switch (org)
        {
        case ErrorOrigin::BUDDY:
            return "buddy allocator";
        case ErrorOrigin::SLAB:
            return "cache's slab";
        case ErrorOrigin::CACHE:
            return "cache";
        case ErrorOrigin::API:
            return "kmem API"; // Will not happen
        default:
            return "unknown";
        }
    }
    const char* MemoryErrorManager::getOrigin() const
    {
        auto org = (Operation)(errorCode & OPERATION_MASK);
        switch (org)
        {
        case Operation::ALLOCATE:
            return "allocating";
        case Operation::DEALLOCATE:
            return "deallocating";
        default:
            return "unknown";
        }
    }
    const char* MemoryErrorManager::getErrorMessage() const
    {
        auto org = (errorCode & CAUSE_MASK);
        switch (org)
        {
        case 0x1:
            return "attempted to deallocate a nullptr";
        case 0x02:
            return "attempted to deallocate foreign address";
        case 0x03:
            if ((errorCode & ORIGIN_MASK) == 0x100)
            {
                return "allocation failed; could not allocated page";
            }
            else
            {
                return "allocation failed; tried to allocate from full page";
            }
        case 0x04:
            return "2**order pages cannot be allocated";
        case 0x05:
            return "tried to deallocate slab's object that was not used at that moment";
        case 0x0D:
            return "slab header could not be allocated for an object that requires a big slab";
        default:
            return "unknown";
        }
    };
    int MemoryErrorManager::ecode() const { return errorCode; }
    const char* MemoryErrorManager::getAPIErrrorMessage(int api_error)
    {
        auto apie = (APIError)(api_error & CAUSE_MASK);
        switch (apie)
        {
        case APIError::DEALLOCATE_NULLPTR:
            return "tried to free \"nullptr\" in kfree\n";
        case APIError::INVALID_DEALLOCATION_ADDRESS:
            return "tried to free ptr not return by kmalloc\n";
        case APIError::NAME_UNDEFINED:
            return "\"nullptr\" was passed instead of cache name\n";
        case APIError::NAME_TOO_LONG:
            return "maximum name length exceeded, maxlen is " STRINGIZE(M_NAME_MAX_LENGTH) "\n";
        case APIError::OUT_OF_SIZE_RANGE:
            return "tried to create a buffer cache that is bigger than 2**" STRINGIZE(BUFFER_MAX_ORDER) "\n";
        case APIError::CACHE_INVALID_CTOR:
            return "you provided a dtor for the cache objects but ctor was not provided\n";
        case APIError::NON_EXISTENT_CACHE:
            return "you attempted an operation on an *cachep* that does not represent a valid kmem_cache_t\n";
        case APIError::INVALID_BUFFER_SIZE:
            return "you attempted to allocate a buffer of size that is not in the premmited range"
                ": [2**" STRINGIZE(BUFFER_MIN_ORDER)", 2**" STRINGIZE(BUFFER_MAX_ORDER) "]\n";
        case APIError::SIZE_N_ALLOC_FAILED:
            return "failed to allocated the appropriate size-N buffer during the kmalloc\n";
        default:
            return "unknown error code encountered\n";
        }
    }

    int MemoryErrorManager::getAPIErrorCode(APIError apie)
    {
        return (int)ErrorOrigin::API | (int)apie;
    }

} // namespace kernel::memory
