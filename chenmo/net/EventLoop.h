#ifndef CHENMO_NET_EVENTLOOP_H
#define CHENMO_NET_EVENTLOOP_H


#include <atomic>
#include <functional>
#include <vector>

#include <boost/any.hpp>

#include "chenmo/base/noncopyable.h"
#include "chenmo/base/Timestamp.h"
#include "chenmo/base/CurrentThread.h"


namespace chenmo {

namespace net {

    class EventLoop: noncopyable {
        public:
            EventLoop();
            ~EventLoop();

            void loop();

            void assertInLoopThread() {
                if (! isInLoopThread())
                {
                    abortNotInLoopThread();
                }
            }

            static EventLoop* getEventLoopOfCurrentThread();


        private:

            bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }
            void abortNotInLoopThread();

            bool looping_;  // 原子操作
            const pid_t threadId_;

    };


}


}



#endif