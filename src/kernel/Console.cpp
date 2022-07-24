//
// Created by os on 7/21/22.
//

#include "../../h/kernel/Console.h"
#include "../../h/kernel/BitMasks.h"
#include "../../h/kernel/RegisterUtils.h"

namespace kernel {
    Console::Console() {
        auto& allocator = MemoryAllocator::getInstance();
        void* stack = allocator.allocateBytes(DEFAULT_STACK_SIZE);
        thread = new Thread(outputTask, nullptr,
                            stack, Thread::Mode::SYSTEM);

        Scheduler::getInstance().put(thread);
    }

    Console &Console::getInstance() {
        static Console instance;
        return instance;
    }

    sem_t Console::getInputSemaphore() {
        return (sem_t) inputItemAvailable;
    }

    char Console::readChar() {
        return inputBuffer.get();
    }

    void Console::writeChar(char c) {
        if(c == '\r') c = '\n';
        outputBuffer.put(c);
    }

    void Console::handle() {
        while (ConsoleController::isReadable() && !inputBuffer.full()) {
            auto c = ConsoleController::receiveData();
            inputBuffer.put(c);
            inputItemAvailable->signal();
            writeChar(c);
        }
    }

    void Console::outputTask(void* ptr) {
        auto& console = getInstance();
        while(true) {
            while(!ConsoleController::isWritable() && console.outputBuffer.empty());
            SREGISTER_CLEAR_BITS(sstatus, BitMasks::sstatus::SIE);
            while (ConsoleController::isWritable() && !console.outputBuffer.empty()) {
                auto c = console.outputBuffer.get();
                ConsoleController::transmitData(c);
            }
            SREGISTER_SET_BITS(sstatus, BitMasks::sstatus::SIE);
        }
    }
} // kernel