#include "../../../h/kernel/Utils/Utils.h"

namespace kernel::utils
{
    uint64 log2(uint64 x)
    {
        if (x == 0) return (uint64)-1;
        if (x == 1) return 0;
        uint64 result = 0;
        while (x > 1)
        {
            x >>= 1;
            result += 1;
        }
        return result;
    }

    char* strcpy(char* dest, const char* src)
    {
        char* ret = dest;
        while (*src != '\0')
        {
            *dest = *src;
            dest++;
            src++;
        }
        *dest = '\0';
        return ret;
    }

    size_t strlen(const char* str)
    {
        size_t len = 0;
        while (*str != '\0')
        {
            len++;
            str++;
        }
        return len;
    }
}