#ifndef SWEET_ASSERT_ASSERT_HPP_INCLUDED
#define SWEET_ASSERT_ASSERT_HPP_INCLUDED

#include <build.hpp>

/**
 Assertion library.

 The assert component provides the macro SWEET_ASSERT(<em>e</em>) for 
 asserting that assumptions are true at runtime. The macro compiles to code
 to generate a __debugbreak() call if the expression is false when the macro
 SWEET_ASSERT_ENABLED is defined and to nothing otherwise.

 An assert function (sweet_assert()) is provided for situations where a macro
 doesn't work.  Lua code uses assertions in expressions in a way that requires
 they be in a function. 
*/

#ifdef __cplusplus
extern "C"
{
#endif

void sweet_break( void );
void sweet_assert( int expression, const char* description, const char* file, int line );
void sweet_assert_with_break( int expression, const char* description, const char* file, int line );
   
#ifdef __cplusplus
}
#endif

#ifdef _MSC_VER
#define SWEET_BREAK() __debugbreak()
#elif defined(BUILD_OS_MACOS)
#define SWEET_BREAK() __asm__("int $3")
#else
#define SWEET_BREAK() sweet_break()
#endif

#ifdef SWEET_ASSERT_ENABLED

#define SWEET_ASSERT( x ) \
do { \
    if ( !(x) ) \
    { \
        sweet_assert( false, #x, __FILE__, __LINE__ ); \
        SWEET_BREAK(); \
    } \
} while ( false )

#else

#ifdef _MSC_VER
#pragma warning( disable: 4127 )
#endif

#define SWEET_ASSERT( x )

#endif

#endif
