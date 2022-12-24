#include "../../../h/kernel/SystemCalls/SystemCallCode.h"
#include "../../../h/kernel/SystemCalls/SystemCalls.h"

namespace kernel
{
    SystemCallCode::SystemCallCode(CallType code):
        code(code)
    { }
    SystemCallType SystemCallCode::getType() const
    {
        return (SystemCallType)((int)code & ~0x0F);
    }
    int SystemCallCode::getOrdinal() const
    {
        return (int)code & 0x0F;
    }
    int SystemCallCode::value() const
    {
        return (int)code;
    }
}