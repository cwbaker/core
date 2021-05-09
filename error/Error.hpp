#ifndef ERROR_ERROR_HPP_INCLUDED
#define ERROR_ERROR_HPP_INCLUDED

#include "macros.hpp"
#include <exception>
#include <stdarg.h>

namespace error
{

/**
// Base class for errors.
*/
class Error : virtual public std::exception
{
    int error_;
    char text_ [1024];

public:
    explicit Error( int error );
    Error( int error, const char* format, ... );
    virtual ~Error() throw ();
    int error() const;
    const char* what() const throw ();
    static const char* format( int oserror, char* buffer, unsigned int length );

protected:
    void append( const char* format, va_list args );
    void append( const char* text );
};

}

#endif
