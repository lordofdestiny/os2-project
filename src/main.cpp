#include "../h/kernel/Kernel.h"

extern void userMain();

void main() {
    using namespace kernel;
    Kernel::initialize();
    userMain();
    Kernel::finalize();
}

