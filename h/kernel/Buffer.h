//
// Created by os on 7/21/22.
//

#ifndef PROJECT_BUFFER_H
#define PROJECT_BUFFER_H

#include "../../lib/hw.h"
#include "./MemoryAllocator.h"

namespace kernel {
    class Buffer {
    public:
        void put(char c)  {
            buffer[head++] = c;
            head %= Capacity;
            count++;
        }
        char get() {
            auto c = buffer[tail++];
            tail %= Capacity;
            count--;
            return c;
        }
        size_t capacity() const {
            return Capacity;
        }
        bool empty() const{
            return count == 0;
        }
        bool full() const{
            return count == Capacity;
        }
    public:
        static void* operator new(size_t size) {
            static auto& allocator = MemoryAllocator::getInstance();
            return allocator.allocateBytes(size);
        }
        static void operator delete(void* ptr) {
            static auto& allocator = MemoryAllocator::getInstance();
            allocator.deallocateBlocks(ptr);
        }
        static const size_t Capacity = 256;
    private:
        size_t head = 0;
        size_t tail = 0;
        size_t count = 0;
        char buffer[Capacity];
    };
};


#endif //PROJECT_BUFFER_H
