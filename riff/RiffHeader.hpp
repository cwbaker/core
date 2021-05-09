#ifndef RIFF_RIFFHEADER_HPP_INCLUDED
#define RIFF_RIFFHEADER_HPP_INCLUDED

namespace riff
{

class RiffHeader
{
public:
    char tag [12];
    unsigned int size;
};

}

#endif
