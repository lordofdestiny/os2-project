#include "../../../h/kernel/Memory/BuddyAllocator.h"
#include "../../../h/kernel/Memory/MemoryManager.h"
#include "../../../h/kernel/Memory/Mapping.h"
#include "../../../h/kernel/Utils/Utils.h"
#include "../../../h/ConsoleUtils.h"
#include "../../../lib/hw.h"

namespace kernel::memory
{
    size_t MemorySection::size() const
    {
        return (size_t)end - (size_t)start;
    }

    MemorySection AllCode()
    {
        return { begin, end };
    }

    MemorySection KernelCode()
    {
        return {
            kernel_text_start,
            kernel_text_end
        };
    }

    MemorySection KernelData()
    {
        return {
            kernel_data_start,
            kernel_data_end
        };
    }

    MemorySection SysCallCode()
    {
        return {
            syscall_text_start,
            syscall_text_end
        };
    }
    MemorySection SysCallData()
    {
        return {
            syscall_data_start,
            syscall_data_end
        };
    }


    MemorySection UserCode()
    {
        return {
            user_code_start,
            user_code_end
        };
    }

    MemorySection UserData()
    {
        return {
            user_data_start,
            user_data_end
        };
    }

    MemorySection Memory()
    {
        return { begin, (void*)PHYSTOP };
    }

    MemorySection UserHeap()
    {
        return {
            (void*)HEAP_START_ADDR,
            (char*)Memory().start + 7 * (Memory().size() >> 3)
        };
    }

    MemorySection KernelHeap()
    {
        return { UserHeap().end,  (void*)PHYSTOP };
    }

} // namespace kernel::memory
