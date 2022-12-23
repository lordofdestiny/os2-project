//
// Created by os on 7/16/22.
//

#ifndef PROJECT_TRAPHANDLERS_H
#define PROJECT_TRAPHANDLERS_H
#include "../../lib/hw.h"
#include "Utils/BitMasks.h"

namespace kernel {
    namespace TrapHandlers{
        using Handler = void(*)(void);

        enum class TrapType : size_t {
            TimerTrap = (size_t) BitMasks::scause::BNT | 0x01,
            ExternalHardwareTrap = (size_t) BitMasks::scause::BNT | 0x09,
            IllegalInstruction = 0x02,
            IllegalReadAddress = 0x05,
            IllegalWriteAddress = 0x07,
            UserEnvironmentCall = 0x08,
            SystemEnvironmentCall = 0x09
        };

        void supervisorTrap() asm("__supervisorTrap__");
        void supervisorTrapHandle() asm("__supervisorTrapHandle__");
        void setErrorHandler(Handler handler);
    };
}

#endif //PROJECT_TRAPHANDLERS_H
