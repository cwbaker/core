
#include <UnitTest++/UnitTest++.h>
#include <riff/RiffWriter.hpp>
#include <riff/RiffHeader.hpp>
#include <error/NullErrorPolicy.hpp>
#include <string.h>

using namespace riff;

SUITE( TestRiffWriter )
{
    TEST( write_length )
    {
        error::NullErrorPolicy error_policy;
        RiffWriter writer( &error_policy );

        writer.reset();
        writer.begin_block( "RSRC" );
        CHECK_EQUAL( 0ul, writer.size() );
        writer.write( 8 );
        CHECK_EQUAL( 8ul, writer.size() );
        writer.end_block();
        CHECK_EQUAL( sizeof(RiffHeader) + 8ul, writer.size() );
    }
    
    TEST( write_data_and_length )    
    {
        error::NullErrorPolicy error_policy;
        RiffWriter writer( &error_policy );

        writer.reset();
        writer.begin_block( "RSRC" );
        CHECK_EQUAL( 0ul, writer.size() );
        char data [8];
        writer.write( data, sizeof(data) );
        CHECK_EQUAL( sizeof(data), writer.size() );
        writer.end_block();
        CHECK_EQUAL( sizeof(RiffHeader) + sizeof(data), writer.size() );
    }
    
    TEST( size_includes_filename_but_not_header )
    {
        error::NullErrorPolicy error_policy;
        RiffWriter writer( &error_policy );

        writer.reset();
        writer.begin_block( "RSRC" );
        writer.end_block();
        
        RiffHeader* header = reinterpret_cast<RiffHeader*>(writer.data());
        CHECK( strncmp(header->tag, "RSRC", 4) == 0 );
        CHECK_EQUAL( 0ul, header->size );
    }
}
