//
// FileSystem.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#define _CRT_SECURE_NO_WARNINGS

#include "FileSystem.hpp"
#include "BasicPath.ipp"
#include <build.hpp>
#include <string>
#if defined(BUILD_OS_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(BUILD_OS_MACOS)
#include <unistd.h>
#include <time.h>
#include <mach-o/dyld.h>
#include <stdlib.h>
#elif defined(BUILD_OS_LINUX)
#include <unistd.h>
#include <linux/limits.h>
#include <sys/sysinfo.h>
#endif

using std::string;
using namespace fs;

FileSystem::FileSystem()
: root_()
, initial_()
, executable_()
, home_()
, directory_stack_( nullptr )
{
    refresh_root();
    refresh_initial();
    refresh_executable();
    refresh_home();
}

const fs::Path& FileSystem::root() const
{
    return root_;
}

const fs::Path& FileSystem::initial() const
{
    return initial_;
}

const fs::Path& FileSystem::executable() const
{
    return executable_;
}

const fs::Path& FileSystem::home() const
{
    return home_;
}

fs::Path FileSystem::root( const fs::Path& path ) const
{
    if ( fs::Path(path).is_absolute() )
    {
        return path;
    }

    fs::Path absolute_path( root_ );
    absolute_path /= path;
    absolute_path.normalize();
    return absolute_path;
}

fs::Path FileSystem::initial( const fs::Path& path ) const
{
    if ( fs::Path(path).is_absolute() )
    {
        return path;
    }

    fs::Path absolute_path( initial_ );
    absolute_path /= path;
    absolute_path.normalize();
    return absolute_path;
}

fs::Path FileSystem::executable( const fs::Path& path ) const
{
    if ( fs::Path(path).is_absolute() )
    {
        return path;
    }

    fs::Path absolute_path( executable_ );
    absolute_path /= path;
    absolute_path.normalize();
    return absolute_path;
}

fs::Path FileSystem::home( const fs::Path& path ) const
{
    if ( fs::Path(path).is_absolute() )
    {
        return path;
    }

    fs::Path absolute_path( home_ );
    absolute_path /= path;
    absolute_path.normalize();
    return absolute_path;
}

std::string FileSystem::root( const char* path ) const
{
    SWEET_ASSERT( path );
    return root( fs::Path(path) ).string();
}

std::string FileSystem::initial( const char* path ) const
{
    SWEET_ASSERT( path );
    return initial( fs::Path(path) ).string();
}

std::string FileSystem::executable( const char* path ) const
{
    SWEET_ASSERT( path );
    return executable( fs::Path(path) ).string();
}

std::string FileSystem::home( const char* path ) const
{
    SWEET_ASSERT( path );
    return home( fs::Path(path) ).string();
}

void FileSystem::set_root( const fs::Path& root )
{
    root_ = root;
}

void FileSystem::set_initial( const fs::Path& initial )
{
    initial_ = initial;
}

void FileSystem::refresh_initial()
{
#if defined(BUILD_OS_WINDOWS)
    char path [MAX_PATH + 1];
    DWORD result = ::GetCurrentDirectory( sizeof(path), path );
    path [sizeof(path) - 1] = 0;
    initial_ = fs::Path( path );
#elif defined(BUILD_OS_MACOS) || defined(BUILD_OS_LINUX)
    char* path = ::getcwd( nullptr, 0 );
    if ( path )
    {
        initial_ = fs::Path( path );
        ::free( path );
    }
#endif
}

void FileSystem::refresh_root()
{
#if defined(BUILD_OS_WINDOWS)
    char path [MAX_PATH + 1];
    DWORD result = ::GetCurrentDirectory( sizeof(path), path );
    path [sizeof(path) - 1] = 0;
    root_ = fs::Path( path );
#elif defined(BUILD_OS_MACOS) || defined(BUILD_OS_LINUX)
    char* path = ::getcwd( NULL, 0 );
    if ( path )
    {
        root_ = fs::Path( path );
        ::free( path );
    }
#endif
}

void FileSystem::refresh_executable()
{
#if defined(BUILD_OS_WINDOWS)
    char path [MAX_PATH + 1];
    int size = ::GetModuleFileNameA( NULL, path, sizeof(path) );
    path [sizeof(path) - 1] = 0;
    executable_ = fs::Path( path ).branch();
#elif defined(BUILD_OS_MACOS)
    uint32_t size = 0;
    _NSGetExecutablePath( NULL, &size );
    char path [size];
    _NSGetExecutablePath( path, &size );
    executable_ = fs::Path( path ).branch();
#elif defined(BUILD_OS_LINUX)
    char path [PATH_MAX];
    ssize_t length = readlink( "/proc/self/exe", path, sizeof(path) );
    if ( length >= 0 )
    {
        executable_ = fs::Path( string(path, length) ).branch();
    }
#endif
}

void FileSystem::refresh_home()
{
#if defined BUILD_OS_WINDOWS
    const char* HOME = "USERPROFILE";
#else
    const char* HOME = "HOME";
#endif    
    const char* home = ::getenv( HOME );
    if (home )
    {
        home_ = fs::Path( string(home) );        
    }
}

void FileSystem::set_directory_stack( DirectoryStack* directory_stack )
{
    directory_stack_ = directory_stack;
}

DirectoryStack* FileSystem::directory_stack() const
{
    return directory_stack_;
}
