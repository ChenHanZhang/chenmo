#include "chenmo/base/CurrentThread.h"
#include "chenmo/base/Logging.h"
#include "chenmo/net/EventLoop.h"


#include <stdio.h>
#include <unistd.h>

using namespace chenmo;

int main() {

    chenmo::Logger::setLogLevel(chenmo::Logger::TRACE);

    printf("main(): pid = %d, tid = %d\n",
    getpid(), chenmo::CurrentThread::tid());

    chenmo::net::EventLoop loop;
    loop.loop();

    return 0;
}