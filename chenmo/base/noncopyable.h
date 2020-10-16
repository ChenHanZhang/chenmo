#ifndef CHENMO_BASE_NONCOPYABLE_H
#define CHENMO_BASE_NONCOPYABLE_H


namespace chenmo {

class noncopyable
{
 public:
  noncopyable(const noncopyable&) = delete;
  void operator=(const noncopyable&) = delete;

 protected:
  noncopyable() = default;
  ~noncopyable() = default;
};

}

#endif