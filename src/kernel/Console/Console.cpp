//
// Created by os on 7/21/22.
//

#include "../../../h/kernel/Console/Console.h"
#include "../../../h/kernel/Utils/BitMasks.h"
#include "../../../h/kernel/Utils/RegisterUtils.h"
#include "../../../h/syscall_c.h"
#include "../../../h/kernel/Kernel.h"

namespace kernel
{
    Console* Console::instance;

    void* Console::operator new(size_t size)
    {
        return kmalloc(size);
    }
    void Console::operator delete(void* ptr)
    {
        return kfree(ptr);
    }

    void Console::initialize()
    {
        instance = new Console();
    }

    Console::Console()
    {
        sem_open(&inputItemAvailable, 0);
        sem_open(&outputSpaceAvailable, BufferSize);
        sem_open(&outputItemAvailable, 0);
        sem_open(&finished, 0);
        inputBuffer = new Buffer<BufferSize>;
        outputBuffer = new Buffer<BufferSize>;
        thread_create(&thread, &outputTask, this);
    };

    Console::~Console()
    {
        sem_close(inputItemAvailable);
        sem_close(outputSpaceAvailable);
        sem_close(outputItemAvailable);
        sem_close(finished);
        delete inputBuffer;
        delete outputBuffer;
    };

    void Console::join()
    {
        sem_wait(finished);
    }

    Console& Console::getInstance()
    {
        return *instance;
    }

    sem_t Console::getInputSemaphore()
    {
        return inputItemAvailable;
    }
    sem_t Console::getOutputSemaphore()
    {
        return outputSpaceAvailable;
    }

    char Console::readChar()
    {
        return inputBuffer->get();
    }

    void Console::writeChar(char c)
    {
        if (c == '\r') c = '\n';
        outputBuffer->put(c);
        ((Semaphore*)outputItemAvailable)->signal();
    }

    void Console::handle()
    {
        while (ConsoleController::isReadable()
            && !inputBuffer->full())
        {
            auto c = ConsoleController::receiveData();
            if (!inputBuffer->full())
            {
                inputBuffer->put(c); // Add handling overfilling buffer
                ((Semaphore*)inputItemAvailable)->signal();
                if (c != 0x1b)
                {
                    writeChar(c);
                }
            }
        }
    }

    void Console::outputTask(void* ptr)
    {
        auto console = (Console*)ptr;
        while (true)
        {
            if (console->outputBuffer != nullptr
                && Thread::isMainFinished()
                && console->outputBuffer->empty())
            {
                break;
            }

            SREGISTER_CLEAR_BITS(sstatus, BitMasks::sstatus::SIE);
            thread_dispatch();
            while (console->outputBuffer != nullptr
                && ConsoleController::isWritable()
                && !console->outputBuffer->empty())
            {
                sem_wait(console->outputItemAvailable);

                const auto c = console->outputBuffer->get();
                ConsoleController::transmitData(c);

                sem_signal(console->outputSpaceAvailable);
            }
            SREGISTER_SET_BITS(sstatus, BitMasks::sstatus::SIE);
        }
        sem_signal(console->finished);
    }

} // kernel
