#include "../h/syscall_c.h"
#include "../h/kernel/MemoryAllocator.h"
#include "../h/kernel/RegisterUtils.h"
#include "../h/kernel/SystemCalls.h"
#include "../h/kernel/Console.h"

#define RETURN_AS(type)                 \
   do {                                 \
        type value;                     \
        REGISTER_READ(a0, value);       \
        return (type) value;            \
   }while(0)                            \

namespace SystemCalls = kernel::SystemCalls;
using SystemCalls::CallType;
using kernel::MemoryAllocator;
using kernel::Console;

void* mem_alloc(size_t size){
    auto blockCount = MemoryAllocator::byteSizeToBlockCount(size);
    REGISTER_WRITE(a1, blockCount);
    environmentCall(CallType::MemoryAllocate);
    RETURN_AS(void*);
}

int mem_free(void* address) {
    REGISTER_WRITE(a1, address);
    environmentCall(CallType::MemoryFree);
    RETURN_AS(int);
}

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg) {
    auto stack_space = mem_alloc(DEFAULT_STACK_SIZE);
    if (stack_space == nullptr) return -0x02;

    REGISTER_WRITE(a4, stack_space);
    REGISTER_WRITE(a3, arg);
    REGISTER_WRITE(a2, start_routine);
    REGISTER_WRITE(a1, handle);

    environmentCall(CallType::ThreadCreate);

    RETURN_AS(int);
}


int thread_exit() {
    environmentCall(CallType::ThreadExit);
    RETURN_AS(int);
}

void thread_dispatch() {
   environmentCall(CallType::ThreadDispatch);
}

int sem_open(sem_t* handle, unsigned init) {
    REGISTER_WRITE(a2, init);
    REGISTER_WRITE(a1, handle);
    environmentCall(CallType::SemaphoreOpen);
    RETURN_AS(int);
}

int sem_close(sem_t handle) {
    REGISTER_WRITE(a1, handle);
    environmentCall(CallType::SemaphoreClose);
    RETURN_AS(int);
}

int sem_wait(sem_t id) {
    REGISTER_WRITE(a1, id);
    environmentCall(CallType::SemaphoreWait);
    RETURN_AS(int);
}

int sem_signal(sem_t id) {
    REGISTER_WRITE(a1, id);
    environmentCall(CallType::SemaphoreSignal);
    RETURN_AS(int);
}

int time_sleep(time_t ticks) {
    REGISTER_WRITE(a1, ticks);
    environmentCall(CallType::TimeSleep);
    RETURN_AS(int);
}

char getc() {
    auto& console = Console::getInstance();
    auto sem = console.getInputSemaphore();
    sem_wait(sem);
    environmentCall(CallType::GetChar);
    RETURN_AS(char);
}

void putc(char c) {
    REGISTER_WRITE(a1, c);
    environmentCall(CallType::PutChar);
}
