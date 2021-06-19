
#include <UnitTest++/UnitTest++.h>
#include <cmdline/Parser.hpp>
#include <error/ErrorPolicy.hpp>

using namespace cmdline;

SUITE( TestParser )
{
    TEST( TestBoolShort )
    {
        bool boolean = false;

        int argc = 2;
        const char* argv[] = { "", "-b" };

        error::ErrorPolicy error_policy;
        Parser command_line_parser;
        command_line_parser.add_options()
            ( "boolean", "b", "Test a boolean option", &boolean )
        ;
        command_line_parser.parse( argc, argv, &error_policy );

        CHECK( boolean );      
    }

    TEST( TestBoolLong )
    {
        bool boolean = false;

        int argc = 2;
        const char* argv[] = { "", "--boolean" };

        error::ErrorPolicy error_policy;
        Parser command_line_parser;
        command_line_parser.add_options()
            ( "boolean", "b", "Test a boolean option", &boolean )
        ;
        command_line_parser.parse( argc, argv, &error_policy );

        CHECK( boolean );      
    }

    TEST( TestIntShort )
    {
        int integer = 0;

        int argc = 2;
        const char* argv[] = { "", "-i1" };

        error::ErrorPolicy error_policy;
        Parser command_line_parser;
        command_line_parser.add_options()
            ( "integer", "i", "Test an integer option", &integer )
        ;
        command_line_parser.parse( argc, argv, &error_policy );

        CHECK( integer == 1 );      
    }

    TEST( TestIntLong )
    {
        int integer = 0;

        int argc = 2;
        const char* argv[] = { "", "--integer=1" };

        error::ErrorPolicy error_policy;
        Parser command_line_parser;
        command_line_parser.add_options()
            ( "integer", "i", "Test an integer option", &integer )
        ;
        command_line_parser.parse( argc, argv, &error_policy );

        CHECK( integer == 1 );      
    }

    TEST( TestFloatShort )
    {
        float real = 0.0f;

        int argc = 2;
        const char* argv[] = { "", "-r1.0" };

        error::ErrorPolicy error_policy;
        Parser command_line_parser;            
        command_line_parser.add_options()
            ( "real", "r", "Test a floating point option", &real )
        ;
        command_line_parser.parse( argc, argv, &error_policy );

        CHECK( real == 1.0f );      
    }

    TEST( TestFloatLong )
    {
        float real = 0.0f;

        int argc = 2;
        const char* argv[] = { "", "--real=1.0" };

        error::ErrorPolicy error_policy;
        Parser command_line_parser;
        command_line_parser.add_options()
            ( "real", "r", "Test a floating point option", &real )
        ;
        command_line_parser.parse( argc, argv, &error_policy );

        CHECK( real == 1.0f );      
    }

    TEST( TestStringShort )
    {
        std::string string;

        int argc = 2;
        const char* argv[] = { "", "-sfoo" };

        error::ErrorPolicy error_policy;
        Parser command_line_parser;
        command_line_parser.add_options()
            ( "string", "s", "Test a string option", &string )
        ;
        command_line_parser.parse( argc, argv, &error_policy );

        CHECK( string == "foo" );      
    }

    TEST( TestStringLong )
    {
        std::string string;

        int argc = 2;
        const char* argv[] = { "", "--string=foo" };

        error::ErrorPolicy error_policy;
        Parser command_line_parser;
        command_line_parser.add_options()
            ( "string", "s", "Test a string option", &string )
        ;
        command_line_parser.parse( argc, argv, &error_policy );

        CHECK( string == "foo" );      
    }
}
