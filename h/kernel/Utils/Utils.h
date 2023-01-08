#ifndef H_PROJECT_UTILS
#define H_PROJECT_UTILS

#include "../../../lib/hw.h"

namespace kernel::utils
{
    uint64 floor_log2(uint64 x);
    uint64 ceil_log2(uint64 x);
    char* strcpy(char* dest, const char* src);
    size_t strlen(const char* str);


    template<typename T>
    struct remove_reference
    {
        typedef T type;
    };

    template<typename T>
    struct remove_reference<T&>
    {
        typedef T type;
    };

    template<typename T>
    struct remove_reference<T&&>
    {
        typedef T type;
    };

    template<typename T>
    using remove_reference_t = typename remove_reference<T>::type;

    template<typename T>
    remove_reference_t<T>&& move(T&& obj)
    {
        return static_cast<remove_reference_t<T>&&>(obj);
    }

    template<typename T>
    constexpr T&& forward(remove_reference_t<T>& obj) noexcept
    {
        return static_cast<T&&>(obj);
    }

    template<typename T>
    constexpr T&& forward(remove_reference_t<T>&& obj) noexcept
    {
        return static_cast<T&&>(obj);
    }

    template<typename T>
    constexpr T&& move(T& obj) noexcept
    {
        return static_cast<T&&>(obj);
    }

    template<typename T, class U = T>
    T exchange(T& obj, U&& newValue)
    {
        T oldValue = move(obj);
        obj = forward<U>(newValue);
        return oldValue;
    }

    void* memset(void* ptr, int value, size_t num);
}

#endif //H_PROJECT_UTILSs