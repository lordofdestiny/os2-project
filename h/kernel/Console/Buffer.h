//
// Created by os on 7/21/22.
//

#ifndef PROJECT_BUFFER_H
#define PROJECT_BUFFER_H

#include "../../../lib/hw.h"
#include "./../Memory/HeapAllocator.h"
#include "./../Memory/slab.h"

namespace kernel
{
    template<size_t Size = 256>
    class Buffer
    {
    public:
        Buffer() { }
        void put(char c)
        {
            buffer[head++] = c;
            head %= Size;
            count++;
        }
        char get()
        {
            auto c = buffer[tail++];
            tail %= Size;
            count--;
            return c;
        }
        size_t capacity() const
        {
            return Size;
        }
        bool empty() const
        {
            return count == 0;
        }
        bool full() const
        {
            return count == Size;
        }
    public:
        static void* operator new(size_t size)
        {
            return kmalloc(size);
        }
        static void operator delete(void* ptr)
        {
            kfree(ptr);
        }
    private:
        size_t head = 0;
        size_t tail = 0;
        size_t count = 0;
        static constexpr size_t HeaderSize = 3 * sizeof(size_t);
        static constexpr size_t Capacity = Size < HeaderSize
            ? 0 : Size - HeaderSize;
        char buffer[Capacity];
    };
};


#endif //PROJECT_BUFFER_H
