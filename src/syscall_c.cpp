#include "../h/syscall_c.h"
#include "../h/kernel/MemoryAllocator.h"
#include "../h/kernel/RegisterUtils.h"

void* mem_alloc(size_t size){
    size_t blockCount = kernel::MemoryAllocator::byteSizeToBlockCount(size);
    WRITE_TO_REGISTER(blockCount, a1);
    asm volatile("li a0, 0x01");

    asm volatile("ecall");

    void* address;
    READ_FROM_REGISTER(a0,address);

    return address;
}

int mem_free(void* address) {
    WRITE_TO_REGISTER(address,a1);
    asm volatile("li a0, 0x02");

    asm volatile("ecall");

    int code;
    READ_FROM_REGISTER(a0, code);
    return code;
}

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg) {
    auto stack_space = mem_alloc(DEFAULT_STACK_SIZE);
    if (stack_space == nullptr) {
        return -0x02;
    }

    WRITE_TO_REGISTER(stack_space, a4);
    WRITE_TO_REGISTER(arg, a3);
    WRITE_TO_REGISTER(start_routine, a2);
    WRITE_TO_REGISTER(handle, a1);
    asm volatile("li a0, 0x11");

    int code;
    READ_FROM_REGISTER(a0,code);
    return code;
}