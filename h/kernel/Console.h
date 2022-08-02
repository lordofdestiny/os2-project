//
// Created by os on 7/21/22.
//

#ifndef PROJECT_CONSOLE_H
#define PROJECT_CONSOLE_H

#include "Semaphore.h"
#include "Buffer.h"
#include "MemoryAllocator.h"
#include "ConsoleController.h"
#include "Scheduler.h"
#include "../syscall_c.h"

#define CONSOLE Console::getInstance()

namespace kernel {
    class Console final{
    public:
        Console()=default;
        Console(Console const&)=delete;
        Console& operator=(Console const&)=delete;
        ~Console()=default;

        void initialize();

        sem_t getInputSemaphore();

        char readChar();
        void writeChar(char c);
        void handle();
    public:
        static Console& getInstance();

    private:
        static void outputTask(void* ptr);
    private:
        sem_t inputItemAvailable;
        sem_t outputItemAvailable;
        Buffer<512> inputBuffer, outputBuffer;
        thread_t thread;
    };

} // kernel

#endif //PROJECT_CONSOLE_H
