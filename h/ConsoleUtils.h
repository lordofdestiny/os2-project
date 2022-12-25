//
// Created by os on 7/18/22.
//

#ifndef PROJECT_CONSOLEUTILS_H
#define PROJECT_CONSOLEUTILS_H
#include "../lib/hw.h"

extern "C" uint64 copy_and_swap(uint64 & lock, uint64 expected, uint64 desired);

void printString(const char* text);

void printHex(uint64 num);

void printReg(const char* name, uint64 value);

char* getString(char* buf, int max);

int stringToInt(const char* s);

void printInt(int xx, int base = 10, int sgn = 0);

void printUInt64(uint64 x, int base = 10);

void printAddress(const void* const address);

#endif //PROJECT_CONSOLEUTILS_H
