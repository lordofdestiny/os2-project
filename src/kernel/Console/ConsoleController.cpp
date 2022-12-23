//
// Created by os on 7/21/22.
//

#include "../../../h/kernel/Console/ConsoleController.h"

namespace kernel {
    uint8 ConsoleController::status() {
        return *(uint8*) CONSOLE_STATUS;
    }

    char ConsoleController::receiveData() {
        return *(char*) CONSOLE_RX_DATA;
    }

    void ConsoleController::transmitData(char data) {
        *((char*)CONSOLE_TX_DATA) = data;
    }

    bool ConsoleController::isReadable() {
        return status() & CONSOLE_RX_STATUS_BIT;
    }

    bool ConsoleController::isWritable() {
        return status() & CONSOLE_TX_STATUS_BIT;
    }
} // kernel