#ifndef CHENMO_BASE_EXCEPTION_H
#define CHENMO_BASE_EXCEPTION_H


#include "chenmo/base/Types.h"
#include <exception>


namespace chenmo
{

class Exception : public std::exception
{
public:
    Exception(string what);
    ~Exception() noexcept override = default;

    const char* what() const noexcept override
    {
        return message_.c_str();
    }

    const char* stackTrace() const noexcept
    {
        return stack_.c_str();
    }

private:
    string message_;
    string stack_;
};


}


#endif