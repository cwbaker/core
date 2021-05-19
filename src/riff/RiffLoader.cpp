//
// RiffLoader.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#define _CRT_SECURE_NO_WARNINGS

#include "RiffLoader.hpp"
#include "RiffPatcher.hpp"
#include "RiffWriter.hpp"
#include "RiffHeader.hpp"
#include "Riff.hpp"
#include <assert/assert.hpp>
#include <algorithm>
#include <stdlib.h>
#include <string.h>

using std::map;
using std::string;
using namespace riff;

RiffLoader::RiffLoader()
: patchers_()
, riffs_()
{
}

RiffLoader::~RiffLoader()
{
    destroy();
}

const std::function<void (const void*)>* RiffLoader::find_patcher( const char* tag ) const
{
    map<string, std::function<void (const void*)>>::const_iterator i = patchers_.find( string(tag) );
    return i != patchers_.end() ? &i->second : nullptr;
}

const Riff* RiffLoader::find_riff( const char* identifier ) const
{
    SWEET_ASSERT( identifier );
    map<string, const Riff*>::const_iterator i = riffs_.find( string(identifier) );
    return i != riffs_.end() ? i->second : nullptr;
}

void RiffLoader::destroy()
{
    for ( map<string, const Riff*>::const_reverse_iterator i = riffs_.rbegin(); i != riffs_.rend(); ++i )
    {
        const Riff* riff = i->second;
        SWEET_ASSERT( riff );
        delete riff;
    }

    patchers_.clear();
    riffs_.clear();
}

void RiffLoader::swap( RiffLoader& riff_loader )
{
    std::swap( patchers_, riff_loader.patchers_ );
    std::swap( riffs_, riff_loader.riffs_ );
}

void RiffLoader::register_patcher( const char* tag, const std::function<void (const void*)>& patcher )
{
    SWEET_ASSERT( tag );
    SWEET_ASSERT( patcher );
    SWEET_ASSERT( !find_patcher(tag) );
    patchers_.insert( make_pair(string(tag), patcher) );
}

const Riff* RiffLoader::load_riff_from_file( const char* filename )
{
    SWEET_ASSERT( filename );
    Riff* riff = new Riff( filename );
    add_riff( filename, riff );
    return riff;
}

const Riff* RiffLoader::load_riff_from_writer( const char* identifier, RiffWriter* riff_writer )
{
    SWEET_ASSERT( identifier );
    SWEET_ASSERT( riff_writer );    
    if ( riff_writer && riff_writer->data() && riff_writer->size() > 0 )
    {
        return load_riff_from_memory( identifier, riff_writer->data(), riff_writer->size() );
    }
    return nullptr;
}

const Riff* RiffLoader::load_riff_from_memory( const char* identifier, unsigned char* data, size_t size )
{
    SWEET_ASSERT( identifier );
    SWEET_ASSERT( data );
    SWEET_ASSERT( size > 0 );    
    if ( identifier && data && size > 0 )
    {
        Riff* riff = new Riff( data, size );
        add_riff( identifier, riff );
        return riff;
    }
    return nullptr;
}

void RiffLoader::unload_riff( const char* identifier )
{
    const Riff* riff = remove_riff( identifier );
    delete riff;
}

void RiffLoader::add_riff( const char* filename, const Riff* riff )
{
    SWEET_ASSERT( filename );
    SWEET_ASSERT( riff );

    string ffilename( filename );
    map<string, const Riff*>::iterator i = riffs_.find( ffilename );
    if ( i == riffs_.end() )
    {
        riffs_.insert( make_pair(ffilename, riff) );
    }
    else
    {
        delete i->second;
        i->second = riff;
    }
    parse_riff( riff );
}

const Riff* RiffLoader::remove_riff( const char* filename )
{
    const Riff* riff = nullptr;
    map<string, const Riff*>::iterator i = riffs_.find( string(filename) );
    SWEET_ASSERT( i != riffs_.end() );
    if ( i != riffs_.end() )
    {
        riff = i->second;
        riffs_.erase( i );
    }
    return riff;
}

void RiffLoader::parse_riff( const Riff* riff )
{
    SWEET_ASSERT( riff );
    for ( RiffIterator i = riff->begin(); i != riff->end(); ++i )
    {
        SWEET_ASSERT( i.size() > 0 );
        const unsigned char* data = i.data();
        
        char tag [12];
#if defined BUILD_OS_LINUX
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-truncation"
#endif
        strncpy( tag, i.tag(), sizeof(tag) );
#if defined BUILD_OS_LINUX
#pragma GCC diagnostic pop
#endif
        const std::function<void (const void*)>* patcher = find_patcher( tag );
        if ( patcher )
        {
            (*patcher)( data );
        }
    }
}
