#ifndef CHENMO_NET_EVENTLOOPTHREAD_H
#define CHENMO_NET_EVENTLOOPTHREAD_H

#include "chenmo/base/Condition.h"
#include "chenmo/base/Mutex.h"
#include "chenmo/base/Thread.h"

namespace chenmo
{
namespace net
{

class EventLoop;

class EventLoopThread
{
public:
    typedef std::function<void(EventLoop*)> ThreadInitCallback;

    EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback(),
                    const string& name = string());
    ~EventLoopThread();
    EventLoop* startLoop();
private:
    void threadFunc();

    EventLoop* loop_ GUARDED_BY(mutex_);
    bool exiting_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_ GUARDED_BY(mutex_);
    ThreadInitCallback callback_;
};

} // end of net

} // end of chenmo

#endif