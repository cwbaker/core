//
// RiffWriter.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#define _CRT_SECURE_NO_WARNINGS

#include "RiffWriter.hpp"
#include "RiffHeader.hpp"
#include <error/ErrorPolicy.hpp>
#include <assert/assert.hpp>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using std::max;
using std::swap;
using namespace riff;

static const size_t INVALID_OFFSET = ~0;

RiffWriter::Block::Block( RiffHeader* hheader, size_t ddata )
: header( hheader )
, data( ddata )
{
}

RiffWriter::RiffWriter( error::ErrorPolicy* error_policy )
: error_policy_( error_policy )
, alignment_( 0 )
, data_( nullptr )
, size_( 0 )
, capacity_( 0 )
, blocks_()
{
    SWEET_ASSERT( error_policy_ );
}

RiffWriter::~RiffWriter()
{
    if ( data_ )
    {
        free( data_ );
        data_ = nullptr;
    }
}

unsigned char* RiffWriter::data() const
{
    return data_;
}

size_t RiffWriter::size() const
{
    return size_ - block_offset();
}

size_t RiffWriter::capacity() const
{
    return capacity_;
}

size_t RiffWriter::block_offset() const
{
    return blocks_.empty() ? 0 : blocks_.back().data;
}

size_t RiffWriter::invalid_offset() const
{
    return INVALID_OFFSET;
}

RiffOffset RiffWriter::offset( size_t offset ) const
{
    if ( offset != INVALID_OFFSET )
    {
        SWEET_ASSERT( block_offset() + offset < size_ );
        return RiffOffset( this, block_offset() + offset );
    }
    return RiffOffset();
}

void* RiffWriter::address( size_t offset ) const
{
    if ( offset != INVALID_OFFSET )
    {
        SWEET_ASSERT( block_offset() + offset < size_ );
        return data_ + block_offset() + offset;
    }
    return nullptr;
}

void RiffWriter::clear()
{
    size_ = 0;   
    blocks_.clear();
}

void RiffWriter::reset( size_t capacity, size_t alignment )
{
    unsigned char* data = reinterpret_cast<unsigned char*>( malloc(capacity * sizeof(unsigned char)) );
    free( data_ );
    alignment_ = alignment;
    data_ = data;
    size_ = 0;
    capacity_ = capacity;
    blocks_.clear();
}

void RiffWriter::align()
{
    align( alignment_ );
}

void RiffWriter::align( size_t alignment )
{
    SWEET_ASSERT( alignment != 0 );
    if ( size_ % alignment != 0 )
    {
        unaligned_write( alignment - size_ % alignment );
    }
    SWEET_ASSERT( size_ % alignment == 0 );
}

void* RiffWriter::write( size_t length )
{
    if ( length > 0 )
    {
        align();
        return unaligned_write( length );
    }
    return nullptr;
}

void* RiffWriter::write( const void* data, size_t length )
{
    if ( length > 0 )
    {
        align();
        return unaligned_write( data, length );
    }
    return nullptr;
}

const char* RiffWriter::write( const std::string& data )
{
    if ( data.size() > 0 )
    {
        align();
        return unaligned_write( data );
    }
    return nullptr;
}

void* RiffWriter::unaligned_write( size_t length )
{
    SWEET_ASSERT( error_policy_ );

    if ( length > 0 )
    {
        int errors = error_policy_->push_errors()
            .error( size_ + length >= capacity_, "Out of space writing %llu bytes to RIFF", length )
        .pop_errors();

        if ( errors == 0 )
        {
            size_t offset = size();
            size_ += length;
            return address( offset );
        }
    }
    return nullptr;
}

void* RiffWriter::unaligned_write( const void* data, size_t length )
{
    if ( length > 0 )
    {
        void* address = write( length );
        memcpy( address, data, length );
        return address;
    }
    return nullptr;
}

const char* RiffWriter::unaligned_write( const std::string& data )
{
    if ( data.size() > 0 )
    {
        return (const char*) unaligned_write( &data[0], data.size() + 1 );
    }
    return nullptr;
}

void* RiffWriter::begin_block( const std::string& tag )
{
    SWEET_ASSERT( !tag.empty() );
    RiffHeader* header = (RiffHeader*) write( sizeof(RiffHeader) );
#if defined BUILD_OS_LINUX
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-truncation"
#endif
    strncpy( header->tag, tag.c_str(), sizeof(header->tag) );
#if defined BUILD_OS_LINUX
#pragma GCC diagnostic pop
#endif
    header->size = 0;
    blocks_.push_back( Block(header, size_) );
    return header;
}

size_t RiffWriter::end_block()
{
    SWEET_ASSERT( !blocks_.empty() );
    align( alignment_ );
    RiffHeader* header = blocks_.back().header;
    size_t size = RiffWriter::size();
    blocks_.pop_back();
    header->size = static_cast<unsigned int>(size);
    return size;
}

void RiffWriter::write_to_file( const std::string& filename )
{
    SWEET_ASSERT( blocks_.empty() );
    SWEET_ASSERT( !filename.empty() );
    
    if ( blocks_.empty() )
    {
        FILE* file = fopen( filename.c_str(), "wb" );
        if ( file )
        {
            fwrite( data(), sizeof(unsigned char), size(), file );
            fclose( file );
            file = nullptr;
        }
    }
}
