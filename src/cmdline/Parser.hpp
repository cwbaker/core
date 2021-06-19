#ifndef CMDLINE_PARSER_HPP_INCLUDED
#define CMDLINE_PARSER_HPP_INCLUDED

#include "Option.hpp"
#include "AddOption.hpp"
#include <string>
#include <vector>
#include <stdio.h>

namespace error
{

class ErrorPolicy;

}

namespace cmdline
{

/**
// A command line options parser.
*/
class Parser
{
    std::vector<Option> options_; ///< The Options that this Parser will parse from the command line.
    std::vector<std::string>* operands_; ///< The vector of strings that this Parser will parse operands from the command line into.

public:
    Parser();
    AddOption add_options();
    bool parse( int argc, char** argv, error::ErrorPolicy* error_policy ) const;
    bool parse( int argc, const char** argv, error::ErrorPolicy* error_policy ) const;
    void print( FILE* stream, int width = 12 ) const;

private:
    bool is_short_option( const char* argument ) const;
    bool is_long_option( const char* argument ) const;
    const Option* find_option_by_name( const std::string& name ) const;
    const Option* find_option_by_short_name( const std::string& short_name ) const;
    const char* find_end_of_name( const char* name ) const;
    const char* find_argument( const char* name_end ) const;
    int parse_option( const Option* option, const char* argument, const char* next_argument, error::ErrorPolicy* error_policy ) const;
};

}

#endif
