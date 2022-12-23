//
// Created by os on 7/21/22.
//

#ifndef PROJECT_BUFFER_H
#define PROJECT_BUFFER_H

#include "../../../lib/hw.h"
#include "./../Memory/HeapAllocator.h"

namespace kernel {
    template<size_t Capacity = 256>
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
            return ALLOCATOR.allocateBytes(size);
        }
        static void operator delete(void* ptr) {
            ALLOCATOR.deallocateBlocks(ptr);
        }
    private:
        size_t head = 0;
        size_t tail = 0;
        size_t count = 0;
        char buffer[Capacity];
    };
};


#endif //PROJECT_BUFFER_H
