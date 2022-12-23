//
// Created by os on 7/15/22.
//

#ifndef PROJECT_BITMASKS_H
#define PROJECT_BITMASKS_H
#include "../../../lib/hw.h"


namespace kernel {
    namespace BitMasks {
        inline constexpr uint64 maskBit(uint8 bit) {
            return (uint64) 0x01 << (uint64) bit;
        }

        enum class sstatus : uint64 {
            SPP = maskBit(8),
            SPIE = maskBit(5),
            SIE = maskBit(1),
        };
        enum class scause : uint64{
            BNT = maskBit(63)
        };
        enum class sip : uint64 {
            SEIP = maskBit(9),
            SSIP = maskBit(1),
        };
        enum class sie : uint64 {
            SEIE = maskBit(9),
            SSIE = maskBit(1),
        };
    }
}


#endif //PROJECT_BITMASKS_H
