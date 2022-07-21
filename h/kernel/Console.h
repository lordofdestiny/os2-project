//
// Created by os on 7/21/22.
//

#ifndef PROJECT_CONSOLE_H
#define PROJECT_CONSOLE_H

#include "Semaphore.h"
#include "BoundedBuffer.h"
#include "ConsoleController.h"
#include "../syscall_c.h"

namespace kernel {
    class Console final{
    public:
        Console() {
        }
        Console(Console const&)=delete;
        Console& operator=(Console const&)=delete;
        ~Console()=default;

        enum class Direction {
            IN, OUT
        };

        sem_t getSemaphore(Direction direction);

        char readChar();
        void writeChar(char c);
        void handle();
    public:
        static Console& getInstance();

        static const size_t BufferSize = 256;
    private:
        Semaphore* outputItemsAvailable = new Semaphore{0};
        Semaphore* inputItemAvailable = new Semaphore{BufferSize};
        BoundedBuffer inputBuffer, outputBuffer;
    };

} // kernel

#endif //PROJECT_CONSOLE_H
