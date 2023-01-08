#ifndef PROJECT_SYSTEMCALLSGROUP_H
#define PROJECT_SYSTEMCALLSGROUP_H

#include "./../Memory/HeapAllocator.h"
#include "../Utils/RegisterUtils.h"
#include "../../../lib/hw.h"

namespace kernel
{

    enum class SystemCallType;
    enum class SystemCallCode;
    using SystemCallHandler = void(*)(void);

    class CallHandlerGroup
    {
        static void* operator new(size_t size);
        static void operator delete(void* ptr);
    public:
        friend class SystemCalls;
        CallHandlerGroup(SystemCallType type);
        CallHandlerGroup(const CallHandlerGroup&) = delete;
        CallHandlerGroup& operator=(const CallHandlerGroup&) = delete;
        ~CallHandlerGroup() = default;
    public:
        bool registerCall(SystemCallCode code, SystemCallHandler routine);
        SystemCallHandler getCall(SystemCallCode code) const;
        bool handles(SystemCallCode type);
    private:
        SystemCallType type;
        SystemCallHandler callHandlers[15];
        static SystemCallType getType(SystemCallCode code);
        static int getCallIndex(SystemCallCode code);
    };
} // namespace kernel

#endif