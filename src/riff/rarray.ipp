#ifndef RIFF_RARRAY_IPP_INCLUDED
#define RIFF_RARRAY_IPP_INCLUDED

#include "rarray.hpp"
#include <assert/assert.hpp>
#include <stdint.h>

namespace riff
{

template <class Type>
rarray<Type>::rarray()
: size_( 0 ),
  offset_( 0 )
{
}

template <class Type>
rarray<Type>::rarray( const rarray& rarray )
: size_( 0 ),
  offset_( 0 )
{
    reset( rarray.size(), rarray.get() );
}

template <class Type>
rarray<Type>::rarray( unsigned int size, void* address )
: size_( 0 ),
  offset_( 0 )
{
    reset( size, address );
}

template <class Type>
rarray<Type>& rarray<Type>::operator=( const rarray& rarray )
{
    reset( rarray.size(), rarray.get() );
    return *this;
}

template <class Type>
const Type* rarray<Type>::begin() const
{
    return get();
}

template <class Type> 
const Type* rarray<Type>::end() const
{
    return get() + size_;
}

template <class Type>
Type& rarray<Type>::front() const
{
    SWEET_ASSERT( size_ > 0 );
    SWEET_ASSERT( offset_ != 0 );
    return get()[0];
}

template <class Type>
Type& rarray<Type>::back() const
{
    SWEET_ASSERT( size_ > 0 );
    SWEET_ASSERT( offset_ != 0 );
    return get()[size_ - 1];
}

template <class Type>
Type& rarray<Type>::operator[]( unsigned int index ) const
{
    SWEET_ASSERT( index < size_ );
    SWEET_ASSERT( offset_ != 0 );
    return get()[index];
}

template <class Type>
Type* rarray<Type>::get() const
{
    return offset_ != 0 ? 
        (Type*) ((intptr_t) this + offset_) : 
        nullptr
    ;
}

template <class Type>
unsigned int rarray<Type>::size() const
{
    return size_;
}

template <class Type>
bool rarray<Type>::empty() const
{
    return size_ == 0;
}

template <class Type>
void rarray<Type>::swap( rarray& rarray )
{
    unsigned int ssize = size();
    Type* address = get();
    reset( rarray.size(), rarray.get() );
    rarray.reset( ssize, address );
}

template <class Type>
void rarray<Type>::reset( unsigned int size, void* address )
{
    if ( size > 0 && address )
    {
        size_ = size;
        offset_ = int((intptr_t) address - (intptr_t) this);
    }
    else
    {
        size_ = 0;
        offset_ = 0;
    }
}

template <class Type>
Type* rarray<Type>::begin()
{
    SWEET_ASSERT( size_ > 0 && offset_ != 0 );
    return get();
}

template <class Type> 
Type* rarray<Type>::end()
{
    SWEET_ASSERT( size_ > 0 && offset_ != 0 );
    return get() + size_;
}

}

#endif
