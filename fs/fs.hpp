#ifndef SWEET_FS_FS_HPP_INCLUDED
#define SWEET_FS_FS_HPP_INCLUDED

#include "BasicPath.hpp"

namespace sweet
{

/**
File System library.

Provides portable paths similar to those provided by boost::filesystem and 
some basic file system operations (copy, remove, exists, etc).
*/
namespace fs
{

Path absolute( const Path& path, const Path& base_path );
Path relative( const Path& path, const Path& base_path );

}

}

#endif
