#ifndef PROJECT_SYSTEMCALLSGROUP_H
#define PROJECT_SYSTEMCALLSGROUP_H

#include "./../Memory/HeapAllocator.h"
#include "../Utils/RegisterUtils.h"
#include "../../../lib/hw.h"

#define CALL_GROUP(Type) SystemCallGroup<SystemCallType::Type>::getInstance()

namespace kernel {

    class SystemCallCode;
    using SystemCallHandler = void(*)(void);

    class SystemCallHandlerGroup {
    public:
        static void* operator new(size_t size);
        static void operator delete(void* ptr);
        SystemCallHandlerGroup(SystemCallType type) : type(type) {
            for (int i = 0; i < 16;callHandlers[i++] = nullptr);
        }
    public:
        bool registerCall(SystemCallCode const& code, SystemCallHandler routine);
        SystemCallHandler getCall(SystemCallCode code) const;
        bool handles(SystemCallCode type);
    private:
        SystemCallHandler callHandlers[16];
        SystemCallType type;
    };
} // namespace kernel

#endif