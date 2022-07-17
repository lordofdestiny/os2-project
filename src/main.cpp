#include "../h/kernel/BitMasks.h"
#include "../h/kernel/RegisterUtils.h"
#include "../h/kernel/TrapHandles.h"
#include "../h/kernel/Collector.h"

void enableInterrupts() {
    using namespace kernel::BitMasks;
    SYS_REGISTER_SET_BITS(sstatus, sstatus::SIE);
}

void disableInterrupts() {
    using namespace kernel::BitMasks;
    SYS_REGISTER_CLEAR_BITS(sstatus, sstatus::SIE);
}

void main() {
    // Set supervisorTrap as a trap handler
    WRITE_TO_SYS_REGISTER(&supervisorTrap, stvec);

    //enableInterrupts();

    void userMain();
    userMain();

    //disableInterrupts();

    kernel::Collector::cleanup();
}

