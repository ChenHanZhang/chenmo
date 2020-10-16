#ifndef CHENMO_BASE_CURRENTTHREAD_H
#define CHENMO_BASE_CURRENTTHREAD_H

#include "chenmo/base/Types.h"

namespace chenmo {

namespace CurrentThread {

    // __thread 是 GCC 内置的线程局部存储设施（Thread-Local Storage），
    // 它的实现非常高效，与 pthread_key_t 向比较更为快速，其存储性能可以与全局变量相媲美，而且使用方式也更为简单。
    // 创建线程局部变量只需简单的在全局或者静态变量的声明中加入__thread 说明即可。

    // 凡是带有__thread 的变量，每个线程都拥有该变量的一份拷贝，且互不干扰。
    // 线程局部存储中的变量将一直存在，直至线程终止，当线程终止时会自动释放这一存储。

    extern __thread int t_cachedTid;
    extern __thread char t_tidString[32];
    extern __thread int t_tidStringLength;
    extern __thread const char* t_threadName;
    void cacheTid();    // 这个函数将会在 Thread.cc 下实现，调用系统的 gettid

    inline int tid()
    {
    // __builtin_expect() 是 GCC (version>= 2.96）提供给程序员使用的，目的是将 “分支转移” 的信息提供给编译器，
    // 这样编译器可以对代码进行优化，以减少指令跳转带来的性能下降。
    // __builtin_expect((x),1) 表示 x 的值为真的可能性更大；
    // __builtin_expect((x),0) 表示 x 的值为假的可能性更大。

    if (__builtin_expect(t_cachedTid == 0, 0))
    {
        cacheTid();
    }
    return t_cachedTid;
    }

    inline const char* tidString() // for logging
    {
    return t_tidString;
    }

    inline int tidStringLength() // for logging
    {
    return t_tidStringLength;
    }

    inline const char* name()
    {
    return t_threadName;
    }

    bool isMainThread();

    void sleepUsec(int64_t usec);  // for testing

    string stackTrace(bool demangle);

}

}

#endif