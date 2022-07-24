//
// Created by os on 7/21/22.
//

#include "../../h/kernel/ConsoleController.h"

namespace kernel {
    uint8 ConsoleController::status() {
        return *(uint64*) CONSOLE_STATUS;
    }
    char ConsoleController::receiveData() {
        return *(uint64*) CONSOLE_RX_DATA;
    }

    void ConsoleController::transmitData(char data) {
        *((uint64*)CONSOLE_TX_DATA) = data;
    }

    bool ConsoleController::isReadable() {
        return status() & CONSOLE_RX_STATUS_BIT;
    }

    bool ConsoleController::isWritable() {
        return status() & CONSOLE_TX_STATUS_BIT;
    }
} // kernel