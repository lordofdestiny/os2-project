//
// Created by os on 7/21/22.
//

#include "../../h/kernel/Console.h"
#include "../../h/kernel/BitMasks.h"
#include "../../h/kernel/RegisterUtils.h"
#include "../../h/syscall_c.h"

namespace kernel {
    Console::Console() {
        void* stack = ALLOCATOR.allocateBytes(DEFAULT_STACK_SIZE);
        thread = new Thread(outputTask, nullptr,
                            stack, Thread::Mode::SYSTEM);
        thread->setStatus(Thread::Status::READY);
        SCHEDULER.put(thread);
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
        outputItemAvailable->signal();
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
        while(true) {
            if(Thread::isMainFished()
            && Thread::threadCount(Thread::Mode::USER) == 1) {
                break;
            }
//            CONSOLE.outputItemAvailable->wait();
            sem_wait((sem_t)CONSOLE.outputItemAvailable);
//            while(!ConsoleController::isWritable() && CONSOLE.outputBuffer.empty());
//            SREGISTER_CLEAR_BITS(sstatus, BitMasks::sstatus::SIE);
            while (ConsoleController::isWritable() && !CONSOLE.outputBuffer.empty()) {
                auto c = CONSOLE.outputBuffer.get();
                ConsoleController::transmitData(c);
            }
//            thread_dispatch();
//            SREGISTER_SET_BITS(sstatus, BitMasks::sstatus::SIE);
        }
    }
} // kernel