#ifndef PROJECT_SYSTEMCALLSCODE_H
#define PROJECT_SYSTEMCALLSCODE_H

namespace kernel
{
    enum class SystemCallType;
    enum class CallType;
    class SystemCallCode
    {
    public:
        SystemCallCode(CallType code);
        SystemCallType getType() const;
        int getOrdinal() const;
        int value() const;
    private:
        CallType code;
    };
}

#endif