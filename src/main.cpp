#include "../h/kernel/Kernel.h"
#include "../h/syscall_c.h"

extern void userMain();

void main()
{
    using namespace kernel;
    Kernel::initialize();

    Kernel::execute(userMain);

    Kernel::finalize(); // Add disposing of all caches via the API?
}

