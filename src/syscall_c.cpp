#include "../h/syscall_c.h"
#include "../h/MemoryAllocator.h"
#include "../h/RegisterUtils.h"

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