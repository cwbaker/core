//
// FileBuilder.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#define _CRT_SECURE_NO_DEPRECATE

#include "FileBuilder.hpp"
#include <sweet/riff/RiffWriter.hpp>
#include <sweet/riff/Riff.hpp>
#include <sweet/riff/File.hpp>
#include <sweet/error/ErrorPolicy.hpp>
#include <sweet/assert/assert.hpp>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace sweet;
using namespace sweet::riff;

#if defined BUILD_OS_WINDOWS
#define _stat32 stat32
#endif

FileBuilder::FileBuilder( RiffWriter& writer, io::IoPolicy* io_policy, error::ErrorPolicy* error_policy )
: writer_( writer ),
  io_policy_( io_policy ),
  error_policy_( error_policy )
{
    SWEET_ASSERT( io_policy_ );
    SWEET_ASSERT( error_policy_ );
}

int FileBuilder::file( const std::string& identifier, const std::string& tag, const std::string& filename )
{
    SWEET_ASSERT( !identifier.empty() );
    SWEET_ASSERT( !filename.empty() );
    SWEET_ASSERT( !tag.empty() );

    error_policy_->push_errors();
    struct stat sstat;
    int result = stat( filename.c_str(), &sstat );
    error_policy_->error( result != 0, "File '%s' not found", filename.c_str() );
    int errors = error_policy_->pop_errors();
    if ( errors == 0 )
    {
        Riff riff( filename.c_str(), io_policy_ );
        if ( riff.data() && riff.size() > 0 )
        {
            for ( RiffIterator i = riff.begin(); i != riff.end(); ++i )
            {
                writer_.begin_block( i.tag() );
                writer_.write( i.data(), i.size() );
                writer_.end_block();            
            }
        }
        else
        {
            unsigned int length = sstat.st_size;

            std::vector<unsigned char> data;
            data.insert( data.end(), length, 0 );
            {
                FILE* file = fopen( filename.c_str(), "rb" );
                SWEET_ASSERT( file );
                size_t read = fread( &data[0], sizeof(unsigned char), length, file );
                (void) read;
                fclose( file );
                file = NULL;
            }

            writer_.begin_block( tag.c_str() );
            File* file = (File*) writer_.write( sizeof(File) );
            file->identifier = writer_.unaligned_write( identifier );
            file->length = static_cast<unsigned int>( data.size() );
            file->data = writer_.write( &data[0], data.size() );
            writer_.end_block();
        }
    }
    return errors;
}
