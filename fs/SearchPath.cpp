//
// SearchPath.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "SearchPath.hpp"
#include <assert/assert.hpp>
#include <filesystem>

using std::string;
using std::filesystem::exists;
using namespace fs;
using namespace std;

SearchPath::SearchPath()
: paths_()
{
}

fs::Path SearchPath::find_path_with_file( const std::string& filename ) const
{
    vector<Path>::const_iterator i = paths_.begin();
    while ( i != paths_.end() && !exists((*i / filename).string()) )
    {
        ++i;
    }
    return i != paths_.end() ? *i : fs::Path();
}

fs::Path SearchPath::which( const std::string& filename ) const
{
    for ( const Path& directory : paths_ )
    {
        std::filesystem::path path( directory.string() );
        path /= filename;
        if ( exists(path) )
        {
            return fs::Path( path.string() );
        }
    }
    return fs::Path();
}

void SearchPath::clear()
{
    paths_.clear();
}

void SearchPath::append_path( const fs::Path& path )
{
    SWEET_ASSERT( !path.empty() );
    paths_.push_back( path );
}

void SearchPath::append_path( const std::string& path )
{
    append_path( Path(path) );
}

void SearchPath::append_path( const char* path )
{
    SWEET_ASSERT( path );
    append_path( Path(path) );
}

void SearchPath::append_paths( const std::vector<std::string>& paths )
{
    for ( const string& path : paths )
    {
        append_path( path );
    }
}

void SearchPath::set_paths( const std::vector<std::string>& paths )
{
    clear();
    append_paths( paths );
}
