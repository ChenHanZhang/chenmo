#include "chenmo/net/EventLoopThread.h"

#include "chenmo/net/EventLoop.h"

using namespace chenmo;
using namespace chenmo::net;

EventLoopThread::EventLoopThread(const ThreadInitCallback& cb,
                                 const string& name)
    : loop_(NULL),
      exiting_(false),
      thread_(std::bind(&EventLoopThread::threadFunc, this), name),
      mutex_(),
      cond_(mutex_),
      callback_(cb)
{
}

EventLoopThread::~EventLoopThread()
{
    exiting_ = true;
    if (loop_ != NULL)
    {
        loop_->quit();
        thread_.join();
    }
}

EventLoop* EventLoopThread::startLoop()
{
    assert(!thread_.started());
    /// start 之后会在子线程中调用 threadFunc
    thread_.start();

    EventLoop* loop = NULL;

    {
        MutexLockGuard lock(mutex_);
        while (loop_ == NULL)
        {
            /// 这里需要等待子线程中将 loop 创建好且执行初始化函数
            cond_.wait();
        }
        loop = loop_;
    }

    return loop;
}

void EventLoopThread::threadFunc()
{
    /// 这个函数已经是在子线程中调用了，要通知父线程
    EventLoop loop;

    if (callback_)
    {
        callback_(&loop);
    }
    
    {
        MutexLockGuard lock(mutex_);
        loop_ = &loop;
        // cond_ notify 之后
        cond_.notify();
    }

    loop.loop();

    MutexLockGuard lock(mutex_);
    loop_ = NULL;
}