//
// Created by os on 7/24/22.
//
#include "../h/syscall_c.h"
#include "../h/syscall_cpp.h"
#include "../h/kernel/Thread.h"

void* operator new (size_t size) {
    return mem_alloc(size);
}

void operator delete (void* ptr) {
    mem_free(ptr);
}

Thread::Thread(void (*body)(void *), void *arg) {
    thread_init(&myHadnle, body, arg);
}

Thread::~Thread()=default;

int Thread::start() {
    return thread_start(&myHadnle);
}

void Thread::dispatch() {
    kernel::Thread::dispatch();
}

int Thread::sleep(time_t time) {
    return time_sleep(time);
}

Thread::Thread() {
    thread_init(&myHadnle,&taskWrapper, this);
}

void Thread::taskWrapper(void *arg) {
    ((Thread*)arg)->run();
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

struct StartingInfo {
    PeriodicThread* thread;
    time_t period;
};

PeriodicThread::PeriodicThread(time_t period) :
    Thread(&taskWrapper, new StartingInfo{this, period}) { }

void PeriodicThread::taskWrapper(void* arg) {
    auto info = (StartingInfo*) arg;
    auto thread = info->thread;
    auto period = info->period;
    while (true) {
        time_sleep(period);
        thread->periodicActivation();
    }
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}

