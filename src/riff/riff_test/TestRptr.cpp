//
// TestRptr.cpp
// Copyright (c) Charles Baker.  All rights reserved
//

#include <UnitTest++/UnitTest++.h>
#include <riff/rptr.hpp>

using namespace riff;

SUITE( Rptr )
{
    TEST( null_on_default_construction )
    {
        rptr<int> rptr;
        CHECK( rptr.get() == nullptr );
        CHECK( !rptr );
    }
    
    TEST( points_to_address_on_explicit_construction )
    {
        int addressed_integer = 0;
        rptr<int> rptr( &addressed_integer );
        CHECK( rptr );
        CHECK( rptr.get() == &addressed_integer );
    }

    TEST( equal_operator )
    {
        int addressed_integer = 0;
        int other_addressed_integer = 0;
        CHECK( rptr<int>(&addressed_integer) == rptr<int>(&addressed_integer) );
        CHECK( !(rptr<int>(&addressed_integer) == rptr<int>(&other_addressed_integer)) );
        CHECK( !(rptr<int>(&addressed_integer) == rptr<int>()) );
    }

    TEST( not_equal_operator )
    {
        int addressed_integer = 0;
        int other_addressed_integer = 0;
        CHECK( rptr<int>(&addressed_integer) != rptr<int>(&other_addressed_integer) );
        CHECK( !(rptr<int>(&addressed_integer) != rptr<int>(&addressed_integer)) );
        CHECK( rptr<int>(&addressed_integer) != rptr<int>() );
    }

    TEST( less_than_operator )
    {
        int integers [2] = { 0, 0 };
        CHECK( rptr<int>(&integers[0]) < rptr<int>(&integers[1]) );
        CHECK( !(rptr<int>(&integers[0]) < rptr<int>(&integers[0])) );
        CHECK( rptr<int>() < rptr<int>(&integers[0]) );
        CHECK( !(rptr<int>(&integers[0]) < rptr<int>()) );
    }
}
