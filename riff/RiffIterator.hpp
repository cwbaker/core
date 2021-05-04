#ifndef SWEET_RIFF_RIFFITERATOR_HPP_INCLUDED
#define SWEET_RIFF_RIFFITERATOR_HPP_INCLUDED

namespace sweet
{
    
namespace riff
{

class RiffIterator
{
    unsigned char* position_;

public:
    RiffIterator();
    RiffIterator( unsigned char* position );
    RiffIterator( const RiffIterator& iterator );
    RiffIterator& operator=( const RiffIterator& iterator );

    bool matches( const char* tag ) const;
    const char* tag() const;
    const unsigned char* data() const;
    unsigned int size() const;
    bool operator!=( const RiffIterator& iterator ) const;

    RiffIterator& operator++();
    RiffIterator operator++( int );
};
    
}

}

#endif
