#include "chenmo/net/Poller.h"
#include "chenmo/net/poller/PollPoller.h"

#include <stdlib.h>

using namespace chenmo::net;

Poller* Poller::newDefaultPoller(EventLoop* loop)
{
    return new PollPoller(loop);
}
