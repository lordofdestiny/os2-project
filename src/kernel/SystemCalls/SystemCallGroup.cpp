#include "../../../h/kernel/SystemCalls/SystemCallCode.h"
#include "../../../h/kernel/SystemCalls/SystemCallGroup.h"
#include "../../../h/kernel/Memory/slab.h"

namespace kernel
{
    void* SystemCallHandlerGroup::operator new(size_t size)
    {
        return kmalloc(size);
    }
    void SystemCallHandlerGroup::operator delete(void* ptr)
    {
        kfree(ptr);
    }

    bool SystemCallHandlerGroup::registerCall(SystemCallCode const& code, SystemCallHandler routine)
    {
        if (type != code.getType())
        {
            return false;
        }
        if (this->callHandlers[code.getOrdinal()] != nullptr)
        {
            return false;
        }
        this->callHandlers[code.getOrdinal()] = routine;
        return true;
    }
    SystemCallHandler SystemCallHandlerGroup::getCall(SystemCallCode code) const
    {
        return this->callHandlers[code.getOrdinal()];
    }
    bool SystemCallHandlerGroup::handles(SystemCallCode type)
    {
        return this->type == type.getType();
    }
}