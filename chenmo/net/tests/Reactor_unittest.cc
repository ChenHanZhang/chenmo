#include "chenmo/net/EventLoop.h"
#include "chenmo/net/Channel.h"
#include "chenmo/base/Timestamp.h"

#include <stdio.h>
#include <sys/timerfd.h>


using namespace chenmo;
using namespace chenmo::net;


EventLoop* g_loop;

void timeout(Timestamp ts)
{
    printf("Timeout!\n");
    g_loop->quit();
}

int main()
{
    EventLoop loop;
    g_loop = &loop;

    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    Channel channel(&loop, timerfd);

    channel.setReadCallback(timeout);
    channel.enableReading();

    struct itimerspec howlong;
    bzero(&howlong, sizeof howlong);
    howlong.it_value.tv_sec = 5;
    ::timerfd_settime(timerfd, 0, &howlong, NULL);

    loop.loop();

    ::close(timerfd);
}