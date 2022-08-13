//
// Created by os on 7/21/22.
//

#include "../../h/kernel/Console.h"
#include "../../h/kernel/BitMasks.h"
#include "../../h/kernel/RegisterUtils.h"
#include "../../h/syscall_c.h"

namespace kernel
{
    Console &Console::getInstance()
    {
        static Console instance;
        return instance;
    }

    sem_t Console::getInputSemaphore()
    {
        return inputItemAvailable;
    }

    char Console::readChar()
    {
        return inputBuffer.get();
    }

    void Console::writeChar(char c)
    {
        if (c == '\r')
            c = '\n';
        outputBuffer.put(c);
        ((Semaphore *)outputItemAvailable)->signal();
    }

    void Console::handle()
    {
        while (ConsoleController::isReadable() && !inputBuffer.full())
        {
            auto c = ConsoleController::receiveData();
            inputBuffer.put(c);
            ((Semaphore *)inputItemAvailable)->signal();
            writeChar(c);
        }
    }

    void Console::outputTask(void *ptr)
    {
        while (true)
        {
            if (Thread::isMainFished() && CONSOLE.outputBuffer.empty())
            {
                break;
            }

            SREGISTER_CLEAR_BITS(sstatus, BitMasks::sstatus::SIE);
            while (ConsoleController::isWritable() && !CONSOLE.outputBuffer.empty())
            {
                sem_wait(CONSOLE.outputItemAvailable);

                auto c = CONSOLE.outputBuffer.get();
                ConsoleController::transmitData(c);
            }
            SREGISTER_SET_BITS(sstatus, BitMasks::sstatus::SIE);
        }
    }

    void Console::initialize()
    {
        thread_create(&thread, &outputTask, nullptr);
        sem_open(&inputItemAvailable, 0);
        sem_open(&outputItemAvailable, 0);
    }
} // kernel
