//
// Riff.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "Riff.hpp"
#include "RiffIterator.hpp"
#include "RiffHeader.hpp"
#include <sweet/io/IoPolicy.hpp>
#include <sweet/assert/assert.hpp>
#include <algorithm>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

using namespace sweet;
using namespace sweet::riff;

Riff::Riff()
: data_( nullptr )
, size_( 0 )
{
}

Riff::Riff( const char* filename, io::IoPolicy* io_policy )
: data_( NULL ),
  size_( 0 )
{
    SWEET_ASSERT( filename );
    SWEET_ASSERT( io_policy );

    void* file = io_policy->fopen( filename, "rb" );
    if ( file )
    {
        RiffHeader header;
        memset( &header, 0, sizeof(header) );
        io_policy->fread( &header, sizeof(header), 1, file );
        
        if ( strcmp(header.tag, "RESOURCE") == 0 )
        {
            data_ = reinterpret_cast<unsigned char*>( malloc(header.size) );
            if ( data_ )
            {
                io_policy->fread( data_, sizeof(unsigned char), header.size, file );
                size_ = header.size;
            }
        }
        
        io_policy->fclose( file );
        file = NULL;        
    }
}

Riff::Riff( unsigned char* data, size_t size )
: data_( NULL ),
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
        data_ = NULL;
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
