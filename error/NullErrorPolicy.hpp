#ifndef SWEET_ERROR_NULLERRORPOLICY_HPP_INCLUDED
#define SWEET_ERROR_NULLERRORPOLICY_HPP_INCLUDED

#include "ErrorPolicy.hpp"

namespace sweet
{

namespace error
{

/**
// Error handler that counts errors but quietly swallows error messages.
*/
class NullErrorPolicy : public ErrorPolicy
{
private:
    void report_error( const char* message ) override;
    void report_print( const char* message ) override;
};

}

}

#endif
