#include "../h/syscall_c.h"
#include "../h/kernel/Memory/HeapAllocator.h"
#include "../h/kernel/Utils/RegisterUtils.h"
#include "../h/kernel/SystemCalls/SystemCalls.h"
#include "../h/kernel/Console/Console.h"

#define RETURN_AS(type)                 \
   do {                                 \
        type value;                     \
        REGISTER_READ(a0, value);       \
        return (type) value;            \
   }while(0)                            \

#define ECALL() asm volatile("ecall")

using CallType = kernel::CallType;
using kernel::memory::HeapAllocator;
using kernel::Console;

void* mem_alloc(size_t size)
{
    auto blockCount = HeapAllocator::byteSizeToBlockCount(size);
    REGISTER_WRITE(a1, blockCount);
    REGISTER_WRITE(a0, CallType::MemoryAllocate);
    ECALL();
    RETURN_AS(void*);
}

int mem_free(void* address)
{
    REGISTER_WRITE(a1, address);
    REGISTER_WRITE(a0, CallType::MemoryFree);
    ECALL();
    RETURN_AS(int);
}

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg)
{
    auto stack_space = mem_alloc(DEFAULT_STACK_SIZE);
    if (stack_space == nullptr) return -0x03;
    REGISTER_WRITE(a4, stack_space);
    REGISTER_WRITE(a3, arg);
    REGISTER_WRITE(a2, start_routine);
    REGISTER_WRITE(a1, handle);
    REGISTER_WRITE(a0, CallType::ThreadCreate);
    ECALL();
    RETURN_AS(int);
}


int thread_exit()
{
    REGISTER_WRITE(a0, CallType::ThreadExit);
    ECALL();
    RETURN_AS(int);
}

void thread_dispatch()
{
    REGISTER_WRITE(a0, CallType::ThreadDispatch);
    ECALL();
}

int thread_init(thread_t* handle, void(*start_routine)(void*), void* arg)
{
    auto stack_space = mem_alloc(DEFAULT_STACK_SIZE);
    if (stack_space == nullptr) return -0x03;
    REGISTER_WRITE(a4, stack_space);
    REGISTER_WRITE(a3, arg);
    REGISTER_WRITE(a2, start_routine);
    REGISTER_WRITE(a1, handle);
    REGISTER_WRITE(a0, CallType::ThreadInit);
    ECALL();
    RETURN_AS(int);
}

int thread_start(thread_t* handle)
{
    if (handle == nullptr) return -0x03;
    REGISTER_WRITE(a1, handle);
    REGISTER_WRITE(a0, CallType::ThreadStart);
    ECALL();
    RETURN_AS(int);
}

void thread_destroy(thread_t* handle)
{
    REGISTER_WRITE(a1, handle);
    REGISTER_WRITE(a0, CallType::ThreadDestroy);
    ECALL();
}

int sem_open(sem_t* handle, unsigned init)
{
    REGISTER_WRITE(a2, init);
    REGISTER_WRITE(a1, handle);
    REGISTER_WRITE(a0, CallType::SemaphoreOpen);
    ECALL();
    RETURN_AS(int);
}

int sem_close(sem_t handle)
{
    REGISTER_WRITE(a1, handle);
    REGISTER_WRITE(a0, CallType::SemaphoreClose);
    ECALL();
    RETURN_AS(int);
}

int sem_wait(sem_t id)
{
    REGISTER_WRITE(a1, id);
    REGISTER_WRITE(a0, CallType::SemaphoreWait);
    ECALL();
    RETURN_AS(int);
}

int sem_signal(sem_t id)
{
    REGISTER_WRITE(a1, id);
    REGISTER_WRITE(a0, CallType::SemaphoreSignal);
    ECALL();
    RETURN_AS(int);
}

int time_sleep(time_t ticks)
{
    REGISTER_WRITE(a1, ticks);
    REGISTER_WRITE(a0, CallType::TimeSleep);
    ECALL();
    RETURN_AS(int);
}

char getc()
{
    sem_wait(CONSOLE.getInputSemaphore());
    REGISTER_WRITE(a0, CallType::GetChar);
    ECALL();
    RETURN_AS(char);
}

void putc(char c)
{
    sem_wait(CONSOLE.getOutputSemaphore());
    REGISTER_WRITE(a1, c);
    REGISTER_WRITE(a0, CallType::PutChar);
    ECALL();
}

void enter_user_mode()
{
    REGISTER_WRITE(a0, CallType::EnterUserMode);
    ECALL();
}