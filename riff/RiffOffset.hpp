#ifndef RIFF_RIFFOFFSET_HPP_INCLUDED
#define RIFF_RIFFOFFSET_HPP_INCLUDED

#include <stddef.h>

namespace riff
{

class RiffWriter;

class RiffOffset
{
    const RiffWriter* writer_;
    size_t offset_;

public:
    RiffOffset();
    RiffOffset( const RiffWriter* writer, size_t offset );
    RiffOffset( const RiffWriter* writer, const void* address );
    RiffOffset( const RiffOffset& offset );
    RiffOffset& operator=( const RiffOffset& offset );

    void* get() const;
    bool operator==( const RiffOffset& RiffOffset ) const;
    bool operator!=( const RiffOffset& RiffOffset ) const;
    bool operator<( const RiffOffset& RiffOffset ) const;
    const RiffWriter* writer() const;

    typedef size_t RiffOffset::* unspecified_bool_type;
    operator unspecified_bool_type() const;

    void swap( RiffOffset& RiffOffset );
    void reset( const RiffWriter* writer, size_t offset );
    void reset( const RiffWriter* writer, const void* address );
};

}

#endif
