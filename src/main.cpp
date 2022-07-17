#include "../h/kernel/BitMasks.h"
#include "../h/kernel/RegisterUtils.h"
#include "../h/kernel/TrapHandler.h"
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
    // Set main trap handler
    using kernel::TrapHandler;
    WRITE_TO_SYS_REGISTER(&TrapHandler::supervisorTrap, stvec);

    //enableInterrupts();

    void userMain();
    userMain();

    //disableInterrupts();

    kernel::Collector::cleanup();
}

