//
// RiffIterator.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "RiffIterator.hpp"
#include "RiffHeader.hpp"
#include <sweet/assert/assert.hpp>
#include <string.h>
#include <stddef.h>

using namespace sweet::riff;

RiffIterator::RiffIterator()
: position_( NULL )
{
}

RiffIterator::RiffIterator( unsigned char* position )
: position_( position )
{
}

RiffIterator::RiffIterator( const RiffIterator& iterator )
: position_( iterator.position_ )
{
}

RiffIterator& RiffIterator::operator=( const RiffIterator& iterator )
{
    if ( &iterator != this )
    {
        position_ = iterator.position_;
    }
    return *this;
}

bool RiffIterator::matches( const char* tag ) const
{
    SWEET_ASSERT( tag );
    const RiffHeader* header = reinterpret_cast<RiffHeader*>( position_ );
    return strncmp( tag, header->tag, sizeof(header->tag) ) == 0;
}

const char* RiffIterator::tag() const
{
    SWEET_ASSERT( position_ );
    const RiffHeader* header = reinterpret_cast<RiffHeader*>( position_ );
    return header ? header->tag : NULL;
}

const unsigned char* RiffIterator::data() const
{
    SWEET_ASSERT( position_ );
    return position_ ? position_ + sizeof(RiffHeader) : NULL;
}

unsigned int RiffIterator::size() const
{
    SWEET_ASSERT( position_ );
    const RiffHeader* header = reinterpret_cast<RiffHeader*>( position_ );
    return header ? header->size : 0;
}

bool RiffIterator::operator!=( const RiffIterator& iterator ) const
{
    return position_ != iterator.position_;
}

RiffIterator& RiffIterator::operator++()
{
    SWEET_ASSERT( position_ );
    position_ += sizeof(RiffHeader) + size();
    return *this;
}

RiffIterator RiffIterator::operator++( int )
{
    RiffIterator iterator( *this );
    position_ += sizeof(RiffHeader) + size();
    return iterator;
}
