//
// Created by os on 7/18/22.
//

#ifndef PROJECT_STACK_H
#define PROJECT_STACK_H
#include "../../lib/hw.h"

const size_t stackSize = DEFAULT_STACK_SIZE*sizeof(uint64);
alignas(uint16) extern uint8 systemStack[stackSize];
extern void* systemStackTopAddress;

#endif //PROJECT_STACK_H