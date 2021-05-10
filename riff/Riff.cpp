//
// Riff.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "Riff.hpp"
#include "RiffIterator.hpp"
#include "RiffHeader.hpp"
#include <assert/assert.hpp>
#include <algorithm>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

using namespace riff;

Riff::Riff()
: data_( nullptr )
, size_( 0 )
{
}

Riff::Riff( const char* filename )
: data_( nullptr ),
  size_( 0 )
{
    SWEET_ASSERT( filename );
    FILE* file = fopen( filename, "rb" );
    if ( file )
    {
        RiffHeader header;
        memset( &header, 0, sizeof(header) );
        size_t read = fread( &header, sizeof(header), 1, file );
        (void) read;
        
        if ( strcmp(header.tag, "RESOURCE") == 0 )
        {
            data_ = reinterpret_cast<unsigned char*>( malloc(header.size) );
            if ( data_ )
            {
                size_t read = fread( data_, sizeof(unsigned char), header.size, file );
                (void) read;
                size_ = header.size;
            }
        }
        
        fclose( file );
        file = nullptr;        
    }
}

Riff::Riff( unsigned char* data, size_t size )
: data_( nullptr ),
  size_( 0 )
{
    data_ = reinterpret_cast<unsigned char*>( malloc(size) );
    size_ = size;
    memcpy( data_, data, size );
}

Riff::~Riff()
{
    if ( data_ )
    {
        free( data_ );
        data_ = nullptr;
    }
}

unsigned char* Riff::data() const
{
    return data_;
}

size_t Riff::size() const
{
    return size_;
}

RiffIterator Riff::begin() const
{
    return RiffIterator( data_ );
}

RiffIterator Riff::end() const
{
    return RiffIterator( data_ + size_ );
}

void Riff::swap( Riff& riff )
{
    std::swap( data_, riff.data_ );
    std::swap( size_, riff.size_ );
}
