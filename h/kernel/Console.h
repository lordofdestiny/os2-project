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

namespace kernel {
    class Console final{
    public:
        Console();
        Console(Console const&)=delete;
        Console& operator=(Console const&)=delete;
        ~Console()=default;

        sem_t getInputSemaphore();
        char readChar();
        void writeChar(char c);
        void handle();
    public:
        static Console& getInstance();

    private:
        [[noreturn]] static void outputTask(void* ptr);
    private:
        Semaphore* inputItemAvailable = new Semaphore{0};
        Buffer<512> inputBuffer, outputBuffer;
        Thread* thread;

    };

} // kernel

#endif //PROJECT_CONSOLE_H
