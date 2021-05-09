#ifndef ERROR_ERRORTEMPLATE_HPP_INCLUDED
#define ERROR_ERRORTEMPLATE_HPP_INCLUDED

#include <stdarg.h>

namespace error
{

/**
// A class template for implementing Error derived classes.
*/
template <int ERRNO, class Base>
class ErrorTemplate : public Base
{
    public:
        ErrorTemplate( const char* format, ... );
        ErrorTemplate( const char* format, va_list args );
};

}

#include "ErrorTemplate.ipp"

#endif
