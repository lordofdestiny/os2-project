#ifndef PROJECT_MEMORY_ERROR_REPORTER_H
#define PROJECT_MEMORY_ERROR_REPORTER_H

namespace kernel::memory
{
    enum class ErrorOrigin
    {
        BUDDY = 0x100,
        SLAB = 0x200,
        CACHE = 0x300,
        API = 0x400
    };

    enum class Operation
    {
        ALLOCATE = 0x010,
        DEALLOCATE = 0x020,
    };

    enum class BuddyError
    {
        DEALLOCATE_NULLPTR = 0x1,
        INVALID_DEALLOCATION_ADDRESS = 0x2,
        ALLOCATION_FAILED = 0x3,
        INVALID_PAGES_ORDER = 0x4,
    };

    enum class SlabError
    {
        DEALLOCATE_NULLPTR = 0x1,
        INVALID_DEALLOCATION_ADDRESS = 0x2,
        ALLOCATION_FAILED = 0x3,
        UNALLOCATED_OBJECT_FREE = 0x5,
        BIG_SLAB_HEADER_ALLOCATION_FAILED = 0xD,
    };

    enum class CacheError
    {
        DEALLOCATE_NULLPTR = 0x1,
        INVALID_DEALLOCATION_ADDRESS = 0x2,
    };

    enum class APIError
    {

        DEALLOCATE_NULLPTR = 0x1,
        INVALID_DEALLOCATION_ADDRESS = 0x2,
        NAME_UNDEFINED = 0x6,
        NAME_TOO_LONG = 0x7,
        OUT_OF_SIZE_RANGE = 0x8,
        CACHE_INVALID_CTOR = 0x9,
        NON_EXISTENT_CACHE = 0xA,
        INVALID_BUFFER_SIZE = 0xB,
        SIZE_N_ALLOC_FAILED = 0xC
    };

    class MemoryErrorManager
    {
    public:
        static constexpr int CAUSE_MASK = 0xF;
        static constexpr int OPERATION_MASK = 0x0F0;
        static constexpr int ORIGIN_MASK = 0xF00;
    public:
        struct ErrorScope
        {
            using Mng = MemoryErrorManager;
            ErrorScope(Mng* m, ErrorOrigin eo, Operation op)
                :mng(m), base_code((int)eo | (int)op), cause(0)
            { }
            ErrorScope(ErrorScope const& es):
                mng(es.mng), base_code(es.base_code),
                cause(0)
            { }
            ErrorScope& operator=(ErrorScope const&) = delete;
            ~ErrorScope()
            {
                if (cause != 0 && mng->errorCode == 0)
                {
                    mng->errorCode = base_code | cause;
                }
            }
            void setError(BuddyError be);
            void setError(SlabError se);
            void setError(CacheError ce);
            Mng* mng;
            int base_code;
            int cause = 0;
        };
        void setOrigin(ErrorOrigin eo);
        void setOperation(Operation op);
        ErrorScope getScope(ErrorOrigin eo, Operation op);
        bool hasError() const;
        const char* getOperation()const;
        const char* getOrigin() const;
        const char* getErrorMessage() const;
        int ecode() const;
        void clear(); // Needs to be called before each cache operation
        static const char* getAPIErrorMessage(int api_error);
        static int getAPIErrorCode(APIError apie);
    private:
        int errorCode;
    };
}

#endif
