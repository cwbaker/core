//
// functions.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "ErrorPolicy.hpp"
#include "functions.hpp"
#include <assert/assert.hpp>
#include <exception>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace error;

namespace error
{

/**
// @internal
//
// The global ErrorPolicy that sweet::error::error() reports Errors to.
*/
static ErrorPolicy* sweet_error_policy = NULL;

/**
// Set the global ErrorPolicy.
//
// @param error_policy
//  The ErrorPolicy to set as the global ErrorPolicy or null to set the global
//  ErrorPolicy to nothing.
*/
void set_error_policy( ErrorPolicy* error_policy )
{
    sweet_error_policy = error_policy;
}

/**
// Get the global ErrorPolicy.
//
// @return
//  The global ErrorPolicy or null if the global ErrorPolicy hasn't been set.
*/
ErrorPolicy* get_error_policy()
{
    return sweet_error_policy;
}

/**
// Handle an error.
//
// Passes the error on to the global ErrorPolicy if one has been set otherwise
// prints the error description to stderr and calls ::exit() passing 
// EXIT_FAILURE as the return code to pass back to the operating system.
//
// @param exception
//  The std::exception object that describes the error that has occured.
*/
void error( const std::exception& exception )
{
    ErrorPolicy* error_policy = error::get_error_policy();
    if ( error_policy )
    {
        error_policy->error( true, exception.what() );
    }
    else
    {
        fputs( exception.what(), stderr );
        fputs( ".\n", stderr );
        exit( EXIT_FAILURE );
    }
}

/**
// Format an operating system error message.
//
// @param error
//  The operating system error number.
// 
// @param buffer
//  A buffer to place the operating system error message into.
// 
// @param length
//  The length of the buffer.
// 
// @return
//  The buffer.
*/
const char* format( int error, char* buffer, unsigned int length )
{
    SWEET_ASSERT( buffer );
#if defined(BUILD_OS_WINDOWS)    
    int actual_length = ::FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM, 0, error, 0, buffer, static_cast<int>(length), 0 );
    while ( actual_length > 0 && (buffer[actual_length] == '\n' || buffer[actual_length] == '\r' || buffer[actual_length] == '.' || buffer[actual_length] == 0) )
    {
        buffer[actual_length] = 0;
        --actual_length;
    }
    return buffer;
#elif defined(BUILD_OS_MACOS)
    strerror_r( error, buffer, length );
    return buffer;
#elif defined(BUILD_OS_LINUX)
    return strerror_r( error, buffer, length );
#else
    (void) error;
    (void) buffer;
    (void) length;
    memset( buffer, 0, length );
    return buffer;
#endif
}

}
