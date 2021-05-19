#ifndef RIFF_RPTR_HPP_INCLUDED
#define RIFF_RPTR_HPP_INCLUDED

namespace riff
{

/**
// A smart pointer using a 32 bit relative offset to refer to an address.
*/
template <class Type>
class rptr
{
    int offset_; ///< The offset from this rptr's this pointer to the address pointed to.

public:
    rptr();
    rptr( const rptr& rptr );
    explicit rptr( const void* address );
    rptr& operator=( const rptr& rptr );
    rptr& operator=( const void* address );

    const Type* operator->() const;
    const Type& operator*() const;
    const Type& operator[]( unsigned int index ) const;
    const Type* get() const;

    bool operator==( const rptr& rptr ) const;
    bool operator!=( const rptr& rptr ) const;
    bool operator<( const rptr& rptr ) const;

    typedef int rptr<Type>::* unspecified_bool_type;
    operator unspecified_bool_type() const;

    void swap( rptr& rptr );
    void reset( const void* pptr = 0 );
};

}

#include "rptr.ipp"

#endif
