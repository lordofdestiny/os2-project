//
// Created by os on 7/15/22.
//

#ifndef PROJECT_BITMASKS_H
#define PROJECT_BITMASKS_H
#include "../../lib/hw.h"

static constexpr size_t maskBit(uint8 bit) {
    return (size_t) 0x01 << (size_t) bit;
}

namespace kernel {
    namespace BitMasks {
        enum class sstatus : size_t {
            SPP = maskBit(8),
            SPIE = maskBit(5),
            SIE = maskBit(1),
        };
        enum class scause : size_t{
            BNT = maskBit(63)
        };
        enum class sip : size_t {
            SEIP = maskBit(9),
            SSIP = maskBit(1),
        };
        enum class sie : size_t {
            SEIE = maskBit(9),
            SSIE = maskBit(1),
        };
    }
}


#endif //PROJECT_BITMASKS_H
