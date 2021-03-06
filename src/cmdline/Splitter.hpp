#ifndef CMDLINE_SPLITTER_HPP_INCLUDED
#define CMDLINE_SPLITTER_HPP_INCLUDED

#include <string>
#include <vector>

namespace cmdline
{

/**
// Split a string into an array of const char* suitable for passing to 
// exec(), posix_spawn(), etc.
*/
class Splitter
{
    std::string command_line_;
    std::vector<char*> arguments_;

public:
    Splitter( const char* command_line );
    const std::vector<char*>& arguments();
};

}

#endif
