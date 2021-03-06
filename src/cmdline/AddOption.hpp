#ifndef CMDLINE_ADDOPTION_HPP_INCLUDED
#define CMDLINE_ADDOPTION_HPP_INCLUDED

#include "Option.hpp"
#include <string>
#include <vector>

namespace cmdline
{

/**
// @internal
//
// A helper class that provides a convenient syntax for adding Options to a
// Parser.
*/
class AddOption
{
    std::vector<Option>* options_; ///< The vector to add Options to.
    std::vector<std::string>** operands_; ///< The pointer to set to point to the vector of strings to store operands to.

public:
    AddOption( std::vector<Option>* options, std::vector<std::string>** operands );
    AddOption& operator()( const std::string& name, const std::string& short_name, const std::string& description, bool* address );
    AddOption& operator()( const std::string& name, const std::string& short_name, const std::string& description, int* address );
    AddOption& operator()( const std::string& name, const std::string& short_name, const std::string& description, float* address );
    AddOption& operator()( const std::string& name, const std::string& short_name, const std::string& description, std::string* address );
    AddOption& operator()( const std::string& name, const std::string& short_name, const std::string& description, std::vector<std::string>* values );
    AddOption& operator()( std::vector<std::string>* operands );
};

}

#endif
