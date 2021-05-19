#ifndef RIFF_FILEBUILDER_HPP_INCLUDED
#define RIFF_FILEBUILDER_HPP_INCLUDED

#include <string>

namespace error
{

class ErrorPolicy;

}

namespace riff
{

class RiffWriter;

class FileBuilder
{
    RiffWriter& writer_;
    error::ErrorPolicy* error_policy_;
    
public:
    FileBuilder( RiffWriter& writer, error::ErrorPolicy* error_policy );
    int file( const std::string& identifier, const std::string& tag, const std::string& filename );
};

}

#endif
