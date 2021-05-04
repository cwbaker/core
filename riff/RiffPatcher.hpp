#ifndef SWEET_RIFF_RIFFPATCHER_HPP_INCLUDED
#define SWEET_RIFF_RIFFPATCHER_HPP_INCLUDED

namespace sweet
{

namespace riff
{

class RiffPatcher
{
public:
    virtual ~RiffPatcher();
    virtual void patch_riff( const void* data, unsigned int size );
};

}

}

#endif
