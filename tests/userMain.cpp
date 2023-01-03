#include "../h/syscall_c.h"
#include "../h/syscall_cpp.hpp"
#include "../h/ConsoleUtils.h"
#include "studentMain.h"
#include "testAllocators.h"
#include "../h/kernel/Memory/BuddyAllocator.h"

void userMain()
{
    // studentMain();
    testMain();
}