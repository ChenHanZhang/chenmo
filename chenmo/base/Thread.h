#ifndef CHENMO_BASE_THREAD_H
#define CHENMO_BASE_THREAD_H


#include "chenmo/base/Atomic.h"
#include "chenmo/base/CountDownLatch.h"
#include "chenmo/base/Types.h"

#include <functional>
#include <memory.h>
#include <pthread.h>

namespace chenmo
{

class Thread : noncopyable
{
public:
    typedef std::function<void ()> ThreadFunc;

    explicit Thread(ThreadFunc, const string& name = string());

    ~Thread();

    void start();
    int join(); // return pthread_join()

    bool started() const { return started_; }

    pid_t tid() const { return tid_; }
    const string& name() const { return name_; }

    static int numCreated() { return numCreated_.get(); }

private:
    void setDefaultName();

    bool        started_;
    bool        joined_;
    pthread_t   pthreadId_;
    pid_t       tid_;
    ThreadFunc  func_;
    string      name_;
    CountDownLatch  latch_;

    static AtomicInt32 numCreated_;
};

} // end of chenmo


#endif