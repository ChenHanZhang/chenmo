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


namespace chenmo {

namespace net {


class Channel;
class Poller;

class EventLoop: noncopyable {
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    void assertInLoopThread() {
        if (! isInLoopThread())
        {
            abortNotInLoopThread();
        }
    }

    static EventLoop* getEventLoopOfCurrentThread();
    bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }
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
    int64_t iteration_;
    const pid_t threadId_;
    
    Timestamp pollReturnTime_;
    std::unique_ptr<Poller> poller_;
    
    ChannelList activeChannels_;
    Channel* currentActiveChannel_;
    
}; // end of class EventLoop


} // end of net


} // end of chenmo



#endif