//
// Created by os on 7/21/22.
//

#include "../../h/kernel/Console.h"
#include "../../h/kernel/BitMasks.h"
#include "../../h/kernel/RegisterUtils.h"
#include "../../h/syscall_c.h"

namespace kernel {
    Console& Console::getInstance() {
        static Console instance;
        return instance;
    }

    void Console::initialize() {
        thread_init(&thread, &outputTask, nullptr);
        sem_open(&inputItemAvailable, 0);
        sem_open(&outputSpaceAvailable, 1024);
        sem_open(&outputItemAvailable, 0);
        sem_open(&finished, 0);
        thread_start(&thread);
    }

    void Console::join() {
        sem_wait(finished);
    }

    sem_t Console::getInputSemaphore() const {
        return inputItemAvailable;
    }
    sem_t Console::getOutputSemaphore() const {
        return outputSpaceAvailable;
    }

    char Console::readChar() {
        return inputBuffer.get();
    }

    void Console::writeChar(char c) {
        if (c == '\r') c = '\n';
        outputBuffer.put(c);
        ((Semaphore*)outputItemAvailable)->signal();
    }

    void Console::handle() {
        while (ConsoleController::isReadable() && !inputBuffer.full()) {
            auto c = ConsoleController::receiveData();
            if (!inputBuffer.full()) {
                inputBuffer.put(c); // Add handling overfilling buffer
                ((Semaphore*)inputItemAvailable)->signal();
                if (c != 0x1b) {
                    writeChar(c);
                }
            }
        }
    }

    void Console::outputTask(void* ptr) {
        while (true) {
            if (Thread::isMainFinished() && CONSOLE.outputBuffer.empty()) {
                sem_signal(CONSOLE.finished);
                break;
            }

            SREGISTER_CLEAR_BITS(sstatus, BitMasks::sstatus::SIE);
            thread_dispatch();
            while (ConsoleController::isWritable() && !CONSOLE.outputBuffer.empty()) {
                sem_wait(CONSOLE.outputItemAvailable);

                auto c = CONSOLE.outputBuffer.get();
                ConsoleController::transmitData(c);

                sem_signal(CONSOLE.outputSpaceAvailable);
            }
            SREGISTER_SET_BITS(sstatus, BitMasks::sstatus::SIE);
        }
    }

} // kernel
