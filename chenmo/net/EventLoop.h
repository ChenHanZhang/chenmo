#ifndef CHENMO_NET_EVENTLOOP_H
#define CHENMO_NET_EVENTLOOP_H


#include <atomic>
#include <functional>
#include <vector>
#include <memory>

#include <boost/any.hpp>

#include "chenmo/base/Mutex.h"
#include "chenmo/base/Timestamp.h"
#include "chenmo/base/CurrentThread.h"
#include "chenmo/net/Callbacks.h"
#include "chenmo/net/TimerId.h"


namespace chenmo {

namespace net {


class Channel;
class Poller;
class TimerQueue;

class EventLoop: noncopyable {
public:
    typedef std::function<void()> Functor;

    EventLoop();
    ~EventLoop();

    // 每一个线程中都有唯一一个 EventLoop， 在 eventloop 中， loop 循环直至 quit
    void loop();

    /// This is not 100% thread safe, if you call through a raw pointer,
    /// better to call through shared_ptr<EventLoop> for 100% safety.
    void quit();

    Timestamp pollReturnTime() const { return pollReturnTime_; }

    int64_t iteration() const { return iteration_; }

    void runInLoop(Functor cb);
    void queueInLoop(Functor cb);
    size_t queueSize() const;

    // timers
    ///
    /// Runs callback at 'time'.
    /// Safe to call from other threads.
    ///
    TimerId runAt(Timestamp time, TimerCallback cb);
    ///
    /// Runs callback after @c delay seconds.
    /// Safe to call from other threads.
    ///
    TimerId runAfter(double delay, TimerCallback cb);
    ///
    /// Runs callback every @c interval seconds.
    /// Safe to call from other threads.
    ///
    TimerId runEvery(double interval, TimerCallback cb);
    ///
    /// Cancels the timer.
    /// Safe to call from other threads.
    ///
    void cancel(TimerId timerId);

    void assertInLoopThread() {
        if (! isInLoopThread())
        {
            abortNotInLoopThread();
        }
    }

    static EventLoop* getEventLoopOfCurrentThread();
    bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }

    void wakeup();
    bool hasChannel(Channel* channel);
    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);

private:

    void abortNotInLoopThread();
    void printActiveChannels() const; // DEBUG

    typedef std::vector<Channel*> ChannelList;

    bool looping_;  // 原子操作
    bool quit_;
    bool eventHandling_;
    bool callingPendingFunctors_; /* atomic */
    int64_t iteration_;
    const pid_t threadId_;
    
    Timestamp pollReturnTime_;
    std::unique_ptr<Poller> poller_;
    std::unique_ptr<TimerQueue> timerQueue_;
    int wakeupFd_;
    std::unique_ptr<Channel> wakeupChannel_;
    
    ChannelList activeChannels_;
    Channel* currentActiveChannel_;

    mutable MutexLock mutex_;
    std::vector<Functor> pendingFunctors_ GUARDED_BY(mutex_);
}; // end of class EventLoop


} // end of net


} // end of chenmo



#endif