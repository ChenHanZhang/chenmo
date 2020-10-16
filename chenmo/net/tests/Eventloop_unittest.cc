#include "chenmo/base/CurrentThread.h"
#include "chenmo/base/Logging.h"
#include "chenmo/base/Thread.h"
#include "chenmo/net/EventLoop.h"


#include <stdio.h>
#include <unistd.h>

using namespace chenmo;
using namespace chenmo::net;

void threadFunc() {
    printf("threadFunc(): pid = %d, tid = %d\n", 
        getpid(), chenmo::CurrentThread::tid());
    
    chenmo::net::EventLoop loop;
    loop.loop();
}


int main() {

    chenmo::Logger::setLogLevel(chenmo::Logger::TRACE);

    printf("main(): pid = %d, tid = %d\n",
    getpid(), chenmo::CurrentThread::tid());

    chenmo::net::EventLoop loop;
    
    // chenmo::net::EventLoop loop2;
    // loop2.loop();

    chenmo::Thread thread(threadFunc);
    thread.start();

    loop.loop();
    pthread_exit(NULL);
    return 0;
}