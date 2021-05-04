#ifndef SWEET_RIFF_FILEBUILDER_HPP_INCLUDED
#define SWEET_RIFF_FILEBUILDER_HPP_INCLUDED

#include <string>

namespace sweet
{

namespace error
{

class ErrorPolicy;

}

namespace io
{

class IoPolicy;

}

namespace riff
{

class RiffWriter;

class FileBuilder
{
    RiffWriter& writer_;
    io::IoPolicy* io_policy_;
    error::ErrorPolicy* error_policy_;
    
public:
    FileBuilder( RiffWriter& writer, io::IoPolicy* io_policy, error::ErrorPolicy* error_policy );
    int file( const std::string& identifier, const std::string& tag, const std::string& filename );
};

}

}

#endif
