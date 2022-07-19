#include "../h/syscall_c.h"
#include "../h/kernel/MemoryAllocator.h"
#include "../h/kernel/RegisterUtils.h"
#include "../h/kernel/SystemCalls.h"

#define RETURN_AS(type)                 \
   do {                                 \
        type value;                     \
        READ_FROM_REGISTER(a0, value);  \
        return (type) value;            \
   }while(0)                            \

namespace SystemCalls = kernel::SystemCalls;
using CallType = SystemCalls::CallType;
using kernel::MemoryAllocator;

void* mem_alloc(size_t size){
    auto blockCount = MemoryAllocator::byteSizeToBlockCount(size);
    WRITE_TO_REGISTER(a1, blockCount);
    environmentCall(CallType::MemoryAllocate);
    RETURN_AS(void*);
}

int mem_free(void* address) {
    WRITE_TO_REGISTER(a1, address);
    environmentCall(CallType::MemoryFree);
    RETURN_AS(int);
}

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg) {
    auto stack_space = mem_alloc(DEFAULT_STACK_SIZE);
    if (stack_space == nullptr) return -0x02;

    WRITE_TO_REGISTER(a4, stack_space);
    WRITE_TO_REGISTER(a3, arg);
    WRITE_TO_REGISTER(a2, start_routine);
    WRITE_TO_REGISTER(a1, handle);

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
    WRITE_TO_REGISTER(a2, init);
    WRITE_TO_REGISTER(a1, handle);
    environmentCall(CallType::SemaphoreOpen);
    RETURN_AS(int);
}

int sem_close(sem_t handle) {
    WRITE_TO_REGISTER(a1, handle);
    environmentCall(CallType::SemaphoreClose);
    RETURN_AS(int);
}

int sem_wait(sem_t id) {
    WRITE_TO_REGISTER(a1, id);
    environmentCall(CallType::SemaphoreWait);
    RETURN_AS(int);
}

int sem_signal(sem_t id) {
    WRITE_TO_REGISTER(a1, id);
    environmentCall(CallType::SemaphoreSignal);
    RETURN_AS(int);
}