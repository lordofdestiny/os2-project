//
// Created by os on 7/18/22.
//

#ifndef PROJECT_SEMAPHORE_H
#define PROJECT_SEMAPHORE_H
#include "./Thread.h"
#include "./Memory/slab.h"

namespace kernel
{
    class Thread;

    class Semaphore final
    {
    public:
        explicit Semaphore(int value);
        Semaphore(Semaphore const&) = delete;
        Semaphore& operator=(Semaphore const&) = delete;
        ~Semaphore();

        void wait();
        void signal();

        long int getValue() const { return value; }
    public:
        static void* operator new(size_t size) noexcept;
        static void operator delete(void* ptr) noexcept;
    private:
        void block();
        void unblock();
        void enqueue(Thread* thread);
        Thread* dequeue();

        long int value;
        Thread* head = nullptr, * tail = nullptr;
        static kmem_cache_t* object_cache;
    public:
        static void initialize();
    };

} // kernel

#endif //PROJECT_SEMAPHORE_H
