#ifndef CHENMO_NET_TIMER_H
#define CHENMO_NET_TIMER_H

#include "chenmo/base/Atomic.h"
#include "chenmo/base/Timestamp.h"
#include "chenmo/net/Callbacks.h"

namespace chenmo
{

namespace net
{

class Timer : noncopyable
{
private:
    const TimerCallback callback_;
    Timestamp expiration_;
    const double interval_;
    const bool repeat_;
    const int64_t sequence_;

    static AtomicInt64 s_numCreated_;
public:
    Timer(TimerCallback cb, Timestamp when, double interval)
      : callback_(std::move(cb)),
        expiration_(when),
        interval_(interval),
        repeat_(interval > 0.0),
        sequence_(s_numCreated_.incrementAndGet())
    {}
    
    void run() const
    {
        callback_();
    }

    Timestamp expiration() const { return expiration_; }
    bool repeat() const { return repeat_; }
    int64_t sequence() const { return sequence_; }

    void restart(Timestamp now);

    static int64_t numCreated() {return s_numCreated_.get();}

}; // end of class Timer


} // end of net

} // end of chenmo


#endif