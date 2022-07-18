#include "../../h/kernel/stack.h"

alignas(uint16) uint8 systemStack[stackSize];
void* systemStackTopAddress = (void*)((uint64) &systemStack + stackSize);