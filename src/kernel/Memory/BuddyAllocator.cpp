#include "../../../h/kernel/Memory/BuddyAllocator.h"
#include "../../../h/kernel/Memory/MemoryManager.h"
#include "../../../h/ConsoleUtils.h"
#include "../../../h/kernel/Utils/Utils.h"

#define MAX_LEVEL 32

#define POW2DIV(num, pow2) (num >> pow2)
#define NBITMASK(n) (~(((uint64)-1)<<n))
#define POW2MOD(num, pow2) (num & NBITMASK(pow2))

namespace kernel::memory
{
    struct FreeBlock
    {
        FreeBlock* prev;
        FreeBlock* next;
    };

    static char* addr_start = nullptr;
    static char* addr_end = nullptr;
    static int num_levels = 0;
    static FreeBlock* freeLists[MAX_LEVEL] = {};
    static char* allocationIndex = nullptr;
    static size_t free_blocks = 0;
    static size_t total_blocks = 0;
    static ErrorManager placeholder{};

    Stats getStats() { return { free_blocks, total_blocks }; }

    inline size_t levelSize(int level);
    inline size_t indexInLevel(void* block, int level);
    inline size_t getIndex(FreeBlock* block, int level);
    inline FreeBlock* getBuddy(FreeBlock* block, int level);

    inline void setAllocated(FreeBlock* block, int level);
    inline void setFree(FreeBlock* block, int level);
    inline bool isFree(size_t index);
    inline bool isBuddyFree(FreeBlock* block, int level);

    FreeBlock* removeBlock(FreeBlock* block, int level)
    {
        if (block == nullptr) return nullptr;
        if (freeLists[level] == nullptr) return nullptr;

        if (freeLists[level] == block)
        {
            freeLists[level] = block->next;
        }
        const auto next = block->next;
        const auto prev = block->prev;

        if (next != nullptr) next->prev = prev;
        if (prev != nullptr) prev->next = next;

        return block;
    }
    void insertBlock(FreeBlock* block, int level)
    {
        auto head = freeLists[level];
        if (head != nullptr)
        {
            head->prev = block;
        }
        block->next = head;
        block->prev = nullptr;
        freeLists[level] = block;
    }
    FreeBlock* allocate(FreeBlock* block, int level)
    {
        setAllocated(block, level);
        return removeBlock(block, level);
    }
    void deallocate(FreeBlock* block, int level)
    {
        setFree(block, level);
        insertBlock(block, level);
    }

    int orderToLevel(int order)
    {
        return num_levels - order - 1;
    }

    void buddy_init(void* space, uint64 blocks_num)
    {
        utils::memset(freeLists, 0, sizeof(freeLists));
        addr_start = (char*)space;
        addr_end = (char*)space + blocks_num * PAGE_SIZE;
        num_levels = utils::floor_log2(blocks_num) + 1;
        total_blocks = blocks_num;
        free_blocks = total_blocks;
        freeLists[0] = (FreeBlock*)space;
        freeLists[0]->prev = nullptr;
        freeLists[0]->next = nullptr;

        // refactor the formula
        auto order = num_levels - PAGE_ORDER - 3;
        if (order < 0) order = 0;
        const int level = orderToLevel(order);

        auto block = removeBlock(freeLists[0], 0);
        for (int lvl = 1; lvl <= level; lvl++)
        {
            auto buddy = getBuddy(block, lvl);
            insertBlock(buddy, lvl);
        }
        allocationIndex = (char*)block;
        free_blocks -= (1 << (order - PAGE_ORDER));

        /*Initialize index*/
        utils::memset(allocationIndex, 0, 1 << (num_levels - 3));
        for (int lvl = 1; lvl < num_levels; lvl++)
        {
            const auto index = (1 << lvl) - 1;
            allocationIndex[POW2DIV(index, 3)] |= 1 << POW2MOD(index, 3);
        }
    }

    FreeBlock* splitInto(int level)
    {
        // Required block already exists, no need to split
        if (freeLists[level] != nullptr)
        {
            return freeLists[level];
        }

        auto blvl = level;
        while (blvl >= 0 && freeLists[blvl] == nullptr)
        {
            blvl--;
        }

        if (blvl < 0) return nullptr;

        auto block = removeBlock(freeLists[blvl], blvl);
        for (auto lvl = blvl; lvl < level; lvl++)
        {
            setAllocated(block, lvl);
            const auto buddy = getBuddy(block, lvl + 1);
            deallocate(buddy, lvl + 1);
        }

        deallocate(block, level);

        return block;
    }
    void insertAndJoin(FreeBlock* block, int level)
    {
        if (not isBuddyFree(block, level))
        {
            deallocate(block, level);
            return;
        };
        for (;isBuddyFree(block, level);level--)
        {
            const auto buddy = getBuddy(block, level);
            allocate(buddy, level);

            block = block < buddy
                ? block
                : buddy;
        }
        deallocate(block, level);
    }

    bool isValidLevel(int level)
    {
        return 0 < level || num_levels < level;
    }

    bool isValidAddress(void* addr)
    {
        return addr_start <= addr
            && addr < addr_end;
    }

    void* page_alloc_impl(int order, int level)
    {
        const auto block = splitInto(level);
        if (block == nullptr) return nullptr;
        free_blocks -= (1 << order);
        return allocate(block, level);
    }

    void* page_alloc(int order)
    {
        const auto level = orderToLevel(order);
        if (not isValidLevel(level)) return nullptr;
        return page_alloc_impl(order, level);
    }

    void* page_alloc(int order,
        ErrorManager& errmng)
    {
        auto escope = errmng.getScope(
            ErrorOrigin::BUDDY,
            Operation::ALLOCATE);
        const auto level = orderToLevel(order);

        if (not isValidLevel(level))
        {
            escope.setError(BuddyError::INVALID_PAGES_ORDER);
            return nullptr;
        }

        const auto block = page_alloc_impl(order, level);
        if (block == nullptr)
        {
            escope.setError(BuddyError::ALLOCATION_FAILED);
        }
        return block;
    }

    int page_free_impl(void* addr, int order)
    {
        if (addr == nullptr) return 1;

        if (not isValidAddress(addr)) return 2;

        const auto level = orderToLevel(order);
        if (not isValidLevel(level)) return 3;

        insertAndJoin((FreeBlock*)addr, level);
        free_blocks += (1 << order);
        return 0;
    }

    void page_free(void* addr, int order)
    {
        page_free_impl(addr, order);
    }

    void page_free(void* addr, int order, ErrorManager& errmng)
    {
        auto escope = errmng.getScope(
            ErrorOrigin::BUDDY,
            Operation::DEALLOCATE);

        if (int code = page_free_impl(addr, order))
        {
            escope.setError((BuddyError)code);
        }
    }

    void printBlockTable()
    {
        for (int i = 0; i < num_levels; i++)
        {
            printInt(i);
            printString(". ");
            printAddress(freeLists[i]);
            if (freeLists[i] != nullptr)
            {
                printString(" || ");
                auto p = freeLists[i];
                while (p->next != nullptr)
                {
                    printAddress(p);
                    printString(", ");
                    p = p->next;
                }
                printAddress(p);
            }
            printString("\n");
        }
    }
    inline size_t levelSize(int level)
    {
        return 1 << (num_levels - 1 + PAGE_ORDER - level);
    }
    inline size_t indexInLevel(void* block, int level)
    {
        return ((char*)block - addr_start) / levelSize(level);
    }
    inline size_t getIndex(FreeBlock* block, int level)
    {
        return (1 << level) + indexInLevel(block, level) - 1;
    }
    inline FreeBlock* getBuddy(FreeBlock* block, int level)
    {
        return (FreeBlock*)((uint64)block ^ levelSize(level));
    }

    inline void setAllocated(FreeBlock* block, int level)
    {
        const auto index = getIndex(block, level);
        allocationIndex[POW2DIV(index, 3)] |= 1 << POW2MOD(index, 3);
    }
    inline void setFree(FreeBlock* block, int level)
    {
        const auto index = getIndex(block, level);
        allocationIndex[POW2DIV(index, 3)] &= ~(1 << POW2MOD(index, 3));
    }
    inline bool isFree(size_t index)
    {
        return (allocationIndex[POW2DIV(index, 3)]
            & (1 << POW2MOD(index, 3))) == 0;
    }
    inline bool isBuddyFree(FreeBlock* block, int level)
    {
        const auto buddy = getBuddy(block, level);
        return isFree(getIndex(buddy, level));
    }
}
