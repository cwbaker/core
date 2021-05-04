//
// RiffOffset.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "RiffOffset.hpp"
#include "RiffWriter.hpp"
#include <stdint.h>
#include <stddef.h>
#include <sweet/assert/assert.hpp>

using namespace sweet::riff;

RiffOffset::RiffOffset()
: writer_( nullptr )
, offset_( 0 )
{
}

RiffOffset::RiffOffset( const RiffWriter* writer, size_t offset )
: writer_( nullptr )
, offset_( 0 )
{
    reset( writer, offset );
}

RiffOffset::RiffOffset( const RiffWriter* writer, const void* address )
: writer_( nullptr )
, offset_( 0 )
{
    reset( writer, address );
}

RiffOffset::RiffOffset( const RiffOffset& offset )
: writer_( nullptr )
, offset_( 0 )
{
    reset( offset.writer_, offset.offset_ );
}

RiffOffset& RiffOffset::operator=( const RiffOffset& offset )
{
    reset( offset.writer_, offset.offset_ );
    return *this;
}

void* RiffOffset::get() const
{
    return writer_ != nullptr ? writer_->data() + offset_ : nullptr;
}

bool RiffOffset::operator==( const RiffOffset& offset ) const
{
    return get() == offset.get();
}

bool RiffOffset::operator!=( const RiffOffset& offset ) const
{
    return get() != offset.get();
}

bool RiffOffset::operator<( const RiffOffset& offset ) const
{
    return get() < offset.get();
}

const RiffWriter* RiffOffset::writer() const
{
    return writer_;
}

RiffOffset::operator unspecified_bool_type() const
{
    return writer_ != nullptr ? &RiffOffset::offset_ : nullptr;
}

void RiffOffset::swap( RiffOffset& offset )
{
    RiffOffset temp( offset );
    offset.reset( writer_, offset_ );
    reset( temp.writer_, temp.offset_ );
}

void RiffOffset::reset( const RiffWriter* writer, size_t offset )
{
    writer_ = writer;
    offset_ = offset;
}

void RiffOffset::reset( const RiffWriter* writer, const void* address )
{
    size_t offset = writer ? (intptr_t) ((const unsigned char*) address - writer->data()) : 0;
    reset( writer, offset );
}
