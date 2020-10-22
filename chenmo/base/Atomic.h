#ifndef CHENMO_BASE_ATOMIC_H
#define CHENMO_BASE_ATOMIC_H

#include "chenmo/base/noncopyable.h"

#include <stdint.h>

namespace chenmo
{

namespace detail
{

template<typename T>
class AtomicIntegerT: noncopyable
{
public:
    AtomicIntegerT()
      : value_(0)
    {
    }

    T get()
    {
        // Gcc 4.1.2 版本之后，对 X86 或 X86_64 支持内置原子操作。
        // 就是说，不需要引入第三方库（如 pthread）的锁保护，
        // 即可对 1、2、4、8 字节的数值或指针类型，进行原子加 / 减 / 与 / 或 / 异或等操作。
        // 有了这套内置原子操作函数，写程序方便很多
        return __sync_val_compare_and_swap(&value_, 0, 0);
    }

    T getAndAdd(T x)
    {
        return __sync_fetch_and_add(&value_, x);
    }

    T addAndGet(T x)
    {
        return getAndAdd(x) + x;
    }

    T incrementAndGet()
    {
        return addAndGet(1);
    }

    T decrementAndGet()
    {
        return addAndGet(-1);
    }

    void add(T x)
    {
        getAndAdd(x);
    }

    void increment()
    {
        incrementAndGet();
    }

    void decrement()
    {
        decrementAndGet();
    }

    T getAndSet(T newValue)
    {
        return __sync_lock_test_and_set(&value_, newValue);
    }

private:
    // volatile 易变不可期的，参考 https://liam.page/2018/01/18/volatile-in-C-and-Cpp/
    volatile T value_;

}; // end of class AtomicIntegerT

} // end of detail

typedef detail::AtomicIntegerT<int32_t> AtomicInt32;
typedef detail::AtomicIntegerT<int64_t> AtomicInt64;

} // end of chenmo


#endif