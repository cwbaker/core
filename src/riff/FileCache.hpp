#ifndef RIFF_FILECACHE_HPP_INCLUDED
#define RIFF_FILECACHE_HPP_INCLUDED

#include <riff/RiffPatcher.hpp>
#include <map>
#include <string>

namespace riff
{

class RiffLoader;
class File;

class FileCache
{
    std::map<std::string, const File*> files_;

public:
    FileCache();
    const File* file( const char* filename ) const;
    void swap( FileCache& file_cache );
    void register_patchers( riff::RiffLoader* loader );

private:
    void patch_file( const void* data );
};

}

#endif
