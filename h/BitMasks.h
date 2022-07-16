//
// Created by os on 7/15/22.
//

#ifndef PROJECT_BITMASKS_H
#define PROJECT_BITMASKS_H
#include "../lib/hw.h"

namespace kernel {
    namespace BitMasks {
        enum class sstatus : size_t {
            SPP = (size_t) 0x01 << 8,
            SPIE = (size_t) 0x01 << 5,
            SIE = (size_t) 0x01<< 1,
        };
        enum class scause : size_t{
            BNT = (size_t) 0x01 << 63
        };
        enum class sip : size_t {
            SEIP = (size_t) 0x01 << 9,
            SSIP = 0x01 << 0x01,
        };
        enum class sie : size_t {
            SEIE = (size_t) 0x01 << 9,
            SSIE = (size_t) 0x01 << 1
        };
    }

    enum class TrapType :size_t {
        TimerTrap = (size_t) BitMasks::scause::BNT | 0x01,
        ExternalHardwareTrap = (size_t) BitMasks::scause::BNT | (size_t) 0x09,
        IllegalInstruction = 0x02,
        IllegalReadAddress = 0x05,
        IllegalWriteAddress = 0x07,
        UserSystemCall = 0x08,
        SystemSystemCall = 0x09
    };
}


#endif //PROJECT_BITMASKS_H
