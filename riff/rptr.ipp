#ifndef SWEET_RIFF_RPTR_IPP_INCLUDED
#define SWEET_RIFF_RPTR_IPP_INCLUDED

#include "rptr.hpp"
#include <stdint.h>
#include <stddef.h>
#include <sweet/assert/assert.hpp>

namespace sweet
{

namespace riff
{

template <class Type>
rptr<Type>::rptr()
: offset_( 0 )
{
}

template <class Type>
rptr<Type>::rptr( const rptr& rptr )
: offset_( 0 )
{
    reset( rptr.get() );
}

template <class Type>
rptr<Type>::rptr( const void* address )
: offset_( 0 )
{
    reset( address );
}

template <class Type>
rptr<Type>& rptr<Type>::operator=( const rptr& rptr )
{
    reset( rptr.get() );
    return *this;
}

template <class Type>
rptr<Type>& rptr<Type>::operator=( const void* address )
{
    reset( address );
    return *this;
}

template <class Type>
const Type* rptr<Type>::operator->() const
{
    return get();
}

template <class Type>
const Type& rptr<Type>::operator*() const
{
    SWEET_ASSERT( offset_ != 0 );
    return *get();
}

template <class Type>
const Type& rptr<Type>::operator[]( unsigned int index ) const
{
    SWEET_ASSERT( offset_ != 0 );
    return get()[index];
}

template <class Type>
const Type* rptr<Type>::get() const
{
    return offset_ != 0 ? 
        (const Type*) ((intptr_t) this + offset_) : 
        nullptr
    ;
}

template <class Type>
bool rptr<Type>::operator==( const rptr& rptr ) const
{
    return get() == rptr.get();
}

template <class Type>
bool rptr<Type>::operator!=( const rptr& rptr ) const
{
    return get() != rptr.get();
}

template <class Type>
bool rptr<Type>::operator<( const rptr& rptr ) const
{
    return get() < rptr.get();
}

template <class Type>
rptr<Type>::operator unspecified_bool_type() const
{
    return offset_ != 0 ? &rptr<Type>::offset_ : NULL;
}

template <class Type>
void rptr<Type>::swap( rptr& rptr )
{
    Type* address = get();
    reset( rptr.get() );
    rptr.reset( address );
}

template <class Type>
void rptr<Type>::reset( const void* address )
{
    offset_ = address ? int((intptr_t) address - (intptr_t) this) : 0;
}

}

}

#endif
