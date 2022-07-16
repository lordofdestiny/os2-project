#include "../h/MemoryAllocator.h"

void* malloc(size_t size){
    static auto& allocator = MemoryAllocator::getInstance();
    size_t blockCount = MemoryAllocator::byteSizeToBlockCount(size);
    return allocator.allocateBlocks(blockCount);
}

int free(void* mem){
    static auto& allocator = MemoryAllocator::getInstance();
    return allocator.freeBlocks(mem);
}

int main() {
    auto arr = (int*) malloc(25*sizeof (int));
    auto arr2 = (short *) malloc(10*sizeof (short ));
    auto pi = (size_t *) malloc(sizeof (size_t ));
    auto x = (size_t *) malloc(sizeof (size_t ));
    *pi = 314;
    *x = 25;
    auto result = 2* *x * *pi;
    auto resul2 = *x * *x * *pi;
    for(int i = 0; i < 10; i++){
        arr2[i]= 2*i;
    }

    for(int i = 0; i < 25; i++){
        arr[i] = i;
    }

    [[maybe_unused]] auto result_3 = result + resul2;

    free(arr);
    free(pi);
    free(x);
    free(arr2);

    return 0;
}

