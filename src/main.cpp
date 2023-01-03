#include "../h/kernel/Kernel.h"
#include "../h/syscall_c.h"
#include "../h/kernel/Memory/MemoryManager.h"
#include "../h/kernel/Memory/BuddyAllocator.h"
#include "../h/kernel/Memory/Cache.h"
#include "../lib/hw.h"
#include "../h/ConsoleUtils.h"
#include "../h/kernel/Utils/Utils.h"
#include "../h/kernel/Memory/slab.h"

extern void userMain();

void main()
{
    using namespace kernel;
    Kernel::initialize();

    Kernel::execute(userMain);

    Kernel::finalize(); // Add disposing of all caches via the API?
}

