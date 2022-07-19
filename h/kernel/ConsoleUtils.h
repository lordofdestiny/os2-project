//
// Created by os on 7/18/22.
//

#ifndef PROJECT_CONSOLEUTILS_H
#define PROJECT_CONSOLEUTILS_H
#include "../../lib/hw.h"


void printString(const char* text);

void printHex(uint64 num);

void printReg(const char* name, uint64 value);

#endif //PROJECT_CONSOLEUTILS_H
