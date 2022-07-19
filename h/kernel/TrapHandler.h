//
// Created by os on 7/16/22.
//

#ifndef PROJECT_TRAPHANDLER_H
#define PROJECT_TRAPHANDLER_H
#include "../../lib/hw.h"
#include "BitMasks.h"

namespace kernel {
    class TrapHandler{
    public:
        static void supervisorTrap();

        static void instructionErrorHandle();

        static void supervisorTrapHandle();

        static void systemCallHandle();

        static void incrementPC();

        enum class TrapType : size_t {
            TimerTrap = (size_t) BitMasks::scause::BNT | 0x01,
            ExternalHardwareTrap = (size_t) BitMasks::scause::BNT | (size_t) 0x09,
            IllegalInstruction = 0x02,
            IllegalReadAddress = 0x05,
            IllegalWriteAddress = 0x07,
            UserEnvironmentCall = 0x08,
            SystemEnvironmentCall = 0x09
        };
    };
}

#endif //PROJECT_TRAPHANDLER_H
