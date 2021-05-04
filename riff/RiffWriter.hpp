#ifndef SWEET_RIFF_RIFFWRITER_HPP_INCLUDED
#define SWEET_RIFF_RIFFWRITER_HPP_INCLUDED

#include "RiffOffset.hpp"
#include <vector>
#include <string>
#include <stddef.h>

namespace sweet
{

namespace error
{

class ErrorPolicy;

}

namespace riff
{

class RiffHeader;

class RiffWriter
{
    struct Block
    {
        RiffHeader* header;
        size_t data;
        Block( RiffHeader* hheader, size_t ddata );
    };

    error::ErrorPolicy* error_policy_;
    size_t alignment_;
    unsigned char* data_;
    size_t size_;
    size_t capacity_;
    std::vector<Block> blocks_;
    
public:
    static const size_t DEFAULT_CAPACITY = 32 * 1024 * 1024;
    static const size_t DEFAULT_ALIGNMENT = 4;

    RiffWriter( error::ErrorPolicy* error_policy );
    ~RiffWriter();
    
    unsigned char* data() const;
    size_t size() const;
    size_t capacity() const;
    size_t block_offset() const;
    size_t invalid_offset() const;
    RiffOffset offset( size_t offset ) const;
    void* address( size_t offset ) const;

    void clear();
    void reset( size_t capacity = DEFAULT_CAPACITY, size_t alignment = DEFAULT_ALIGNMENT );
    void align();
    void align( size_t alignment );
    void* write( size_t length );
    void* write( const void* data, size_t length );
    const char* write( const std::string& data );
    void* unaligned_write( size_t length );
    void* unaligned_write( const void* data, size_t length );
    const char* unaligned_write( const std::string& data );
    void* begin_block( const std::string& tag );
    size_t end_block();
    void write_to_file( const std::string& filename );
};

}

}

#endif
