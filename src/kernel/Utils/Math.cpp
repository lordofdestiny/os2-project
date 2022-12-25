#include "../../../h/kernel/Utils/Math.h"

namespace kernel
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
}