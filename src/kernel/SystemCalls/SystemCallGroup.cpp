#include "../../../h/kernel/SystemCalls/SystemCallGroup.h"
#include "../../../h/kernel/Memory/slab.h"

namespace kernel
{
    void* CallHandlerGroup::operator new(size_t size)
    {
        return kmalloc(size);
    }
    void CallHandlerGroup::operator delete(void* ptr)
    {
        kfree(ptr);
    }
    CallHandlerGroup::CallHandlerGroup(SystemCallType type)
        : type(type)
    {
        for (int i = 0; i < 15;callHandlers[i++] = nullptr);
    }

    SystemCallType CallHandlerGroup::getType(SystemCallCode code)
    {
        return (SystemCallType)((int)code & ~0x0F);
    }

    int CallHandlerGroup::getCallIndex(SystemCallCode code)
    {
        return ((int)code & 0x0F) - 1;
    }

    bool CallHandlerGroup::registerCall(SystemCallCode code, SystemCallHandler routine)
    {
        if (type != getType(code)) return false;
        const auto index = getCallIndex(code);

        if (callHandlers[index] != nullptr) return false;
        callHandlers[index] = routine;

        return true;
    }
    SystemCallHandler CallHandlerGroup::getCall(SystemCallCode code) const
    {
        return callHandlers[getCallIndex(code)];
    }
    bool CallHandlerGroup::handles(SystemCallCode type)
    {
        return this->type == getType(type);
    }
}