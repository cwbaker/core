#ifndef RIFF_RIFFPATCHER_HPP_INCLUDED
#define RIFF_RIFFPATCHER_HPP_INCLUDED

namespace riff
{

class RiffPatcher
{
public:
    virtual ~RiffPatcher();
    virtual void patch_riff( const void* data, unsigned int size );
};

}

#endif
