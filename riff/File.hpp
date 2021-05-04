#ifndef SWEET_RIFF_FILE_HPP_INCLUDED
#define SWEET_RIFF_FILE_HPP_INCLUDED

#include "rptr.hpp"

namespace sweet
{

namespace riff
{

class File
{
public:
    rptr<const char> identifier;
    unsigned int length;
    rptr<const char> data;
};

}

}

#endif
