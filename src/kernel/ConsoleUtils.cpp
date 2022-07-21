//
// Created by os on 7/18/22.
//
#include "../../h/kernel/ConsoleUtils.h"
#include "../../lib/console.h"

void printString(const char* text) {
    auto ptr = (char*)text;
    while(*ptr != '\0') __putc(*ptr++);
}

void printHex(uint64 num) {
    char str[20];
    str[0] = '0';
    str[1] = 'x';

    auto digits = "0123456789ABCDEF";

    for(int i =0; i < 16; i++) {
        uint64 cdigit = num % 16;
        str[17-i] = digits[cdigit];
        num >>= 4;
    }

    str[18] = '\n';
    str[19] = '\0';

    for(auto i = 0 ; i < 19; i++){
        __putc(str[i]);
    }
}

void printReg(const char* name, uint64 value) {
    printString(name);
    printString(": ");
    printHex(value);
}
