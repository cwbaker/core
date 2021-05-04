#ifndef SWEET_RIFF_RIFFHEADER_HPP_INCLUDED
#define SWEET_RIFF_RIFFHEADER_HPP_INCLUDED

namespace sweet
{

namespace riff
{

class RiffHeader
{
public:
    char tag [12];
    unsigned int size;
};

}

}

#endif
