//
// riffc.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "Application.hpp"
#include <exception>
#include <stdlib.h>

using namespace riff;

int main( int argc, char** argv )
{
    try
    {
        Application application( argc, argv );
        return application.result();
    }

    catch ( const std::exception& exception )
    {
        fprintf( stderr, "riffc: %s.\n", exception.what() );
        return EXIT_FAILURE;      
    }

    catch ( ... )
    {
        fprintf( stderr, "riffc: An unexpected error occured.\n" );
        return EXIT_FAILURE;      
    }
}
