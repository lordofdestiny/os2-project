//
// Created by os on 7/21/22.
//

#include "../../h/kernel/Console.h"

namespace kernel {
    Console &Console::getInstance() {
        static Console instance;
        return instance;
    }

    char Console::readChar() {
        return inputBuffer.get();
    }

    void Console::writeChar(char c) {
        outputBuffer.put(c);
    }

    sem_t Console::getSemaphore(Console::Direction direction) {
        switch (direction) {
            case Direction::IN: return (sem_t) inputItemAvailable;
            case Direction::OUT: return (sem_t) outputItemsAvailable;
            default: return nullptr;
        }
    }

    void Console::handle() {
        while (ConsoleController::isReadable() && !outputBuffer.empty()) {
            auto c = outputBuffer.get();
            ConsoleController::transmitData(c);
            outputItemsAvailable->signal();
        }
        while (ConsoleController::isReadable() && !inputBuffer.full()) {
            auto c = ConsoleController::receiveData();
            inputBuffer.put(c);
            inputItemAvailable->signal();
        }
    }

} // kernel