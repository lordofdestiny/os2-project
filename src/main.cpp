#include "../h/BitMasks.h"
#include "../h/RegisterUtils.h"
#include "../h/TrapHandles.h"

void enableInterrupts() {
    using namespace kernel::BitMasks;
    SYS_REGISTER_SET_BITS(sstatus, sstatus::SIE);
}

void main() {
    // Set supervisorTrap as a trap handler
    WRITE_TO_SYS_REGISTER(&supervisorTrap, stvec);

    //enableInterrupts();

    void userMain();
    userMain();
}

