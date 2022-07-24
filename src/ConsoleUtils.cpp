//
// Created by os on 7/18/22.
//
#include "../h/ConsoleUtils.h"
#include "../h/syscall_c.h"

uint64 lockPrint = 0;

#define LOCK() while(copy_and_swap(lockPrint, 0, 1))
#define UNLOCK() while(copy_and_swap(lockPrint, 1, 0))

void printString(const char *text) {
    LOCK();
    auto ptr = (char *) text;
    while (*ptr != '\0') putc(*ptr++);
    UNLOCK();
}

void printHex(uint64 num) {
    char str[20];
    str[0] = '0';
    str[1] = 'x';

    auto digits = "0123456789ABCDEF";

    for (int i = 0; i < 16; i++) {
        uint64 cdigit = num % 16;
        str[17 - i] = digits[cdigit];
        num >>= 4;
    }

    str[18] = '\n';
    str[19] = '\0';

    for (auto i = 0; i < 19; i++) {
        putc(str[i]);
    }
}

void printReg(const char *name, uint64 value) {
    printString(name);
    printString(": ");
    printHex(value);
}

char* getString(char *buf, int max) {
    LOCK();
    int i, cc;
    char c;

    for(i=0; i+1 < max; ){
        cc = getc();
        if(cc < 1)
            break;
        c = cc;
        buf[i++] = c;
        if(c == '\n' || c == '\r')
            break;
    }
    buf[i] = '\0';

    UNLOCK();
    return buf;
}

int stringToInt(const char *s) {
    int n;

    n = 0;
    while ('0' <= *s && *s <= '9')
        n = n * 10 + *s++ - '0';
    return n;
}

char digits[] = "0123456789ABCDEF";

void printInt(int xx, int base, int sgn)
{
    LOCK();
    char buf[16];
    int i, neg;
    uint x;

    neg = 0;
    if(sgn && xx < 0){
        neg = 1;
        x = -xx;
    } else {
        x = xx;
    }

    i = 0;
    do{
        buf[i++] = digits[x % base];
    }while((x /= base) != 0);
    if(neg)
        buf[i++] = '-';

    while(--i >= 0)
        putc(buf[i]);

    UNLOCK();
}
