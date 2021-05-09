#ifndef RIFF_RIFF_HPP_INCLUDED
#define RIFF_RIFF_HPP_INCLUDED

#include "RiffIterator.hpp"
#include <stddef.h>

namespace riff
{

class Riff
{
    unsigned char* data_;
    size_t size_;

public:
    Riff();
    Riff( const char* filename );
    Riff( unsigned char* data, size_t size );
    ~Riff();
    unsigned char* data() const;
    size_t size() const;
    RiffIterator begin() const;
    RiffIterator end() const;
    void swap( Riff& riff );

private:
    Riff( const Riff& riff ) = delete;
    Riff& operator=( const Riff& riff ) = delete;
};
    
}

#endif
