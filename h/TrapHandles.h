//
// Created by os on 7/16/22.
//

#ifndef PROJECT_TRAPHANDLES_H
#define PROJECT_TRAPHANDLES_H
#include "../lib/hw.h"
#include "../h/BitMasks.h"

extern "C" void supervisorTrap();

extern "C" void supervisorTrapHandle();

namespace kernel {
    void instructionErrorHandle();

    enum class TrapType : size_t {
        TimerTrap = (size_t) BitMasks::scause::BNT | 0x01,
        ExternalHardwareTrap = (size_t) BitMasks::scause::BNT | (size_t) 0x09,
        IllegalInstruction = 0x02,
        IllegalReadAddress = 0x05,
        IllegalWriteAddress = 0x07,
        UserSystemCall = 0x08,
        SystemSystemCall = 0x09
    };
}

#endif //PROJECT_TRAPHANDLES_H
