#include "chenmo/base/Exception.h"
#include "chenmo/base/CurrentThread.h"


namespace chenmo
{
    Exception::Exception(string msg)
      : message_(std::move(msg)),
      stack_(CurrentThread::stackTrace(false))
      {
      }
}