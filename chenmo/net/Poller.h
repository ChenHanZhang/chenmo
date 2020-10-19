#ifndef CHENMO_NET_POLL
#define CHENMO_NET_POLL

#include <map>
#include <vector>

#include "chenmo/base/Timestamp.h"
#include "chenmo/net/EventLoop.h"

namespace chenmo
{

namespace net
{

class Channel;

class Poller
{

/// 注意，Poller只是一个虚基类，具体的实现在 chenmo/net/poller 下

private:
    EventLoop* ownerLoop_;

protected:
    typedef std::map<int, Channel*> ChannelMap;
    ChannelMap channels_;

public:
    typedef std::vector<Channel*> ChannelList;

    Poller(EventLoop* loop);
    virtual ~Poller();

    /// Polls the I/O events.
    /// Must be called in the loop thread.
    virtual Timestamp poll(int timeoutMs, ChannelList* activeChannels) = 0;

    /// Changes the interested I/O events.
    /// Must be called in the loop thread.
    virtual void updateChannel(Channel* channel) = 0;

    /// Remove the channel, when it destructs.
    /// Must be called in the loop thread.
    virtual void removeChannel(Channel* channel) = 0;

    virtual bool hasChannel(Channel* channel) const;

    static Poller* newDefaultPoller(EventLoop* loop);

    void assertInLoopThread() const
    {
        ownerLoop_->assertInLoopThread();
    }
};


} // end of chenmo


} // end of chenmo


#endif