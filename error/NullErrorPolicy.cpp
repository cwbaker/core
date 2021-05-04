//
// NullErrorPolicy.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "NullErrorPolicy.hpp"

using namespace sweet::error;

void NullErrorPolicy::report_error( const char* /*message*/ )
{
}

void NullErrorPolicy::report_print( const char* /*message*/ )
{
}
