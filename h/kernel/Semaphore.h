//
// Created by os on 7/18/22.
//

#ifndef PROJECT_SEMAPHORE_H
#define PROJECT_SEMAPHORE_H
#include "./Thread.h"

namespace kernel {
    class Semaphore final{
    public:
        explicit Semaphore(int value);
        Semaphore(Semaphore const&)=delete;
        Semaphore& operator=(Semaphore const&)=delete;
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
        Thread *head= nullptr, *tail= nullptr;
    };

} // kernel

#endif //PROJECT_SEMAPHORE_H
