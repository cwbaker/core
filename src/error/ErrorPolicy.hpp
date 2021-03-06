#ifndef ERROR_ERRORPOLICY_HPP_INCLUDED
#define ERROR_ERRORPOLICY_HPP_INCLUDED

#include <stdarg.h>

namespace std
{

class exception;

}

namespace error
{

/**
// An interface for providing runtime error handling policies.
*/
class ErrorPolicy
{
    static const int ERROR_STACK_SIZE = 32;
    int stack_index_;
    int errors_ [ERROR_STACK_SIZE];

public:
    ErrorPolicy();
    virtual ~ErrorPolicy();
    void clear();
    ErrorPolicy& push_errors();
    int pop_errors();
    int errors() const;
    ErrorPolicy& error( bool condition, const char* format, ... );
    void print( const char* format, ... );
    void error_varargs( bool condition, const char* format, va_list args );
    void print_varargs( const char* format, va_list args );

private:
    virtual void report_error( const char* message );
    virtual void report_print( const char* message );
};

}

#endif
