//
// Created by os on 7/24/22.
//

#ifndef _syscall_cpp
#define _syscall_cpp

#include "syscall_c.h"

void* operator new (size_t size);
void operator delete (void* ptr);

class Thread
{
public:
    Thread(void (*body)(void*), void* arg);
    virtual ~Thread();

    int start();

    static void dispatch();
    static int sleep(time_t);

protected:
    Thread();
    virtual void run() { }

private:
    thread_t myHandle = nullptr;

    static void taskWrapper(void* arg);
};

class Semaphore
{
public:

    Semaphore(unsigned init = 1);
    virtual ~Semaphore();

    int wait();
    int signal();

private:
    sem_t myHandle;
};

class PeriodicThread: public Thread
{
protected:
    PeriodicThread(time_t period);
    virtual void periodicActivation() { };
private:
    static void taskWrapper(void*);
};

class Console
{
public:
    static char getc();
    static void putc(char);
};

#endif //_syscall_cpp
