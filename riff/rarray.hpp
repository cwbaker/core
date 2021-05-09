#ifndef RIFF_RARRAY_HPP_INCLUDED
#define RIFF_RARRAY_HPP_INCLUDED

namespace riff
{

/**
// A smart array using a 32 bit relative offset to refer to the array of data.
*/
template <class Type>
class rarray
{
    unsigned int size_; ///< The number of elements in the array.
    int offset_; ///< The offset from this rarray's this pointer to the address pointed to.

public:
    typedef const Type* const_iterator;

    rarray();
    rarray( const rarray& rarray );
    explicit rarray( unsigned int size, void* address );
    rarray& operator=( const rarray& rarray );

    const Type* begin() const;
    const Type* end() const;
    Type& front() const;
    Type& back() const;
    Type& operator[]( unsigned int index ) const;
    Type* get() const;
    unsigned int size() const;
    bool empty() const;

    void swap( rarray& rarray );
    void reset( unsigned int size, void* array = 0 );
    Type* begin();
    Type* end();
};

}

#include "rarray.ipp"

#endif
