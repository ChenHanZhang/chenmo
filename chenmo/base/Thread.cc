#include "chenmo/base/Thread.h"
#include "chenmo/base/CurrentThread.h"
#include "chenmo/base/Exception.h"
#include "chenmo/base/Logging.h"

#include <type_traits>

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>

namespace chenmo
{

namespace detail
{

pid_t gettid()
{
    // 系统调用获取 pid
    return static_cast<pid_t>(::syscall(SYS_gettid));
}


void afterFork()
{
    chenmo::CurrentThread::t_cachedTid = 0;
    chenmo::CurrentThread::t_threadName = "main";
    CurrentThread::tid();
}

class ThreadNameInitializer
{
public:
    ThreadNameInitializer()
    {
        chenmo::CurrentThread::t_threadName = "main";
        CurrentThread::tid();
        pthread_atfork(NULL, NULL, &afterFork);
    }
};

ThreadNameInitializer init;

struct ThreadData
{
    typedef chenmo::Thread::ThreadFunc ThreadFunc;
    ThreadFunc func_;
    string name_;
    pid_t* tid_;
    CountDownLatch* latch_;

    ThreadData(ThreadFunc func,
               const string& name,
               pid_t* tid,
               CountDownLatch* latch)
    : func_(std::move(func)),
      name_(name),
      tid_(tid),
      latch_(latch)
    {}

    void runInThread()
    {
        *tid_ = chenmo::CurrentThread::tid();
        tid_ = NULL;
        latch_->countDown();
        latch_ = NULL;

        chenmo::CurrentThread::t_threadName = name_.empty() ? "chenmoThread" : name_.c_str();
        // 给线程命名
        ::prctl(PR_SET_NAME, chenmo::CurrentThread::t_threadName);

        try
        {
            func_();
            chenmo::CurrentThread::t_threadName = "finished";
        }
        catch(const Exception& ex)
        {
            chenmo::CurrentThread::t_threadName = "crashed";
            fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
            fprintf(stderr, "reason: %s\n", ex.what());
            fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
            abort();
        }
        catch (const std::exception& ex)
        {
            chenmo::CurrentThread::t_threadName = "crashed";
            fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
            fprintf(stderr, "reason: %s\n", ex.what());
            abort();
        }
        catch (...)
        {
            chenmo::CurrentThread::t_threadName = "crashed";
            fprintf(stderr, "unknown exception caught in Thread %s\n", name_.c_str());
            throw; // rethrow
        }
    }
};

void* startThread(void* obj)
{
    ThreadData* data = static_cast<ThreadData*>(obj);
    data->runInThread();
    delete data;
    return NULL;
}

} // end of detail

void CurrentThread::cacheTid()
{
    if (t_cachedTid == 0)
    {
        t_cachedTid = detail::gettid();
        t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
    }
}

bool CurrentThread::isMainThread()
{
    return tid() == ::getpid();
}

void CurrentThread::sleepUsec(int64_t usec)
{
  struct timespec ts = { 0, 0 };
  ts.tv_sec = static_cast<time_t>(usec / Timestamp::kMicroSecondsPerSecond);
  ts.tv_nsec = static_cast<long>(usec % Timestamp::kMicroSecondsPerSecond * 1000);
  ::nanosleep(&ts, NULL);
}

AtomicInt32 Thread::numCreated_;

Thread::Thread(ThreadFunc func, const string& n)
  : started_(false),
    joined_(false),
    pthreadId_(0),
    tid_(0),
    func_(std::move(func)),
    name_(n),
    latch_(1)
{
    setDefaultName();
}

Thread::~Thread()
{
    if (started_ && !joined_)
    {
        pthread_detach(pthreadId_);
    }
    
}

void Thread::setDefaultName()
{
    int num = numCreated_.incrementAndGet();
    if (name_.empty())
    {
        char buf[32];
        snprintf(buf, sizeof buf, "Thread%d", num);
        name_ = buf;
    }
}

void Thread::start()
{
    assert(!started_);
    started_ = true;

    detail::ThreadData* data = new detail::ThreadData(func_, name_, &tid_, &latch_);

    // pthread_create:
    // 第一个参数为指向线程标识符的指针（例如：pthread_t p_thread）
    // 第二个参数用来设置线程属性
    // 第三个参数是线程运行函数的起始地址
    // 第四个参数是运行函数的参数

    // pthread_create 会执行 detail::startThread 函数，并且以 data 作为参数
    // 其中， data 中有 latch_ 变量，这个变量是当前 Thread 的成员变量的别名，
    // 在 startThread 函数中，将 data 转为 ThreadData 类型，然后调用其 runInThread 函数，
    // 在 runInThread 函数中， latch_ 将会被 countDown()，这样，下面 else 中的 latch_.wait() 得以返回
    if (pthread_create(&pthreadId_, NULL, &detail::startThread, data))
    {
        started_ = false;
        delete data;
        LOG_SYSFATAL << "Failed in pthread_create";
    }else{
        // 这里在父线程中会等待子线程中将 latch_ 减一
        // 当进入到这里时，子线程已经启动了，父线程会空等至 pthread_create 调用了函数
        latch_.wait();
        assert(tid_ > 0);
    }
    
}

int Thread::join()
{
  assert(started_);
  assert(!joined_);
  joined_ = true;
  // pthread_join 会阻塞的等待参数中的线程结束，然后才返回
  return pthread_join(pthreadId_, NULL);
}

} // end of chenmo