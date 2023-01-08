//
// Created by os on 7/21/22.
//

#ifndef PROJECT_CONSOLE_H
#define PROJECT_CONSOLE_H

#include "../Semaphore.h"
#include "./Buffer.h"
#include "../Memory/HeapAllocator.h"
#include "./ConsoleController.h"
#include "../Scheduler.h"
#include "../../syscall_c.h"

#define CONSOLE Console::getInstance()

namespace kernel
{
    class Console final
    {
    public:
        friend class Kernel;
        Console(Console const&) = delete;
        Console& operator=(Console const&) = delete;
        ~Console();

        void join();
        static Console& getInstance();

        sem_t getInputSemaphore();
        sem_t getOutputSemaphore();

        char readChar();
        void writeChar(char c);
        void handle();

        static void initialize();
    private:
        static void* operator new(size_t size);
        static void operator delete(void* ptr);
        static constexpr size_t BufferSize = 1024;
        Console();
        static void outputTask(void* ptr);
    private:
        sem_t inputItemAvailable;
        sem_t outputSpaceAvailable;
        sem_t outputItemAvailable;
        sem_t finished;
        Buffer<BufferSize>* inputBuffer;
        Buffer<BufferSize>* outputBuffer;
        thread_t thread;
        static Console* instance;
    };

} // kernel

#endif //PROJECT_CONSOLE_H
