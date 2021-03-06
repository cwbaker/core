//
// DirectoryStack.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "DirectoryStack.hpp"
#include "BasicPath.ipp"
#include <assert/assert.hpp>

using namespace fs;

/**
// Constructor.
*/
DirectoryStack::DirectoryStack()
: directories_()
{
}

/**
// Reset the working directory stack to contain only \e directory.
//
// @param directory
//  The directory to set the working directory stack to contain (assumed
//  to be absolute or empty).
*/
void DirectoryStack::reset_directory( const fs::Path& directory )
{
    SWEET_ASSERT( directory.empty() || directory.is_absolute() );
    directories_.clear();
    directories_.push_back( directory );
}

/**
// Change the current working directory to \e directory.
//
// If \e directory is a relative path then it is considered to be relative
// to the current working directory and an absolute path is constructed by
// combining the current working directory and \e directory.
//
// @param directory
//  The directory to change the current working directory to.
*/
void DirectoryStack::change_directory( const fs::Path& directory )
{
    SWEET_ASSERT( !directories_.empty() );

    if ( directory.is_absolute() )
    {
        directories_.back() = directory;
    }
    else
    {
        directories_.back() /= directory;
        directories_.back().normalize();
    }
}

/**
// Push \e directory as the current working directory.
//
// The previous current working directory can be returned to by calling
// DirectoryStack::pop_directory().
//
// If \e directory is a relative path then it is considered to be relative
// to the current working directory and an absolute path is constructed by
// combining the current working directory and \e directory.
//
// @param directory
//  The directory to change the current working directory to.
*/
void DirectoryStack::push_directory( const fs::Path& directory )
{
    SWEET_ASSERT( !directories_.empty() );

    if ( directory.is_absolute() )
    {
        directories_.push_back( directory );
    }
    else
    {
        directories_.push_back( directories_.back() / directory );
        directories_.back().normalize();
    }
}

/**
// Pop the current working directory to return to the previous working 
// directory.
//
// If there is only the current working directory in the directory stack then
// this function silently does nothing.
*/
void DirectoryStack::pop_directory()
{
    if ( directories_.size() > 1 )
    {
        directories_.pop_back();
    }
}

/**
// Get the current working directory.
//
// @return
//  The current working directory.
*/
const fs::Path& DirectoryStack::directory() const
{
    SWEET_ASSERT( !directories_.empty() );
    return directories_.back();
}
