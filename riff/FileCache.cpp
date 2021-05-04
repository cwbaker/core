
#include "FileCache.hpp"
#include "File.hpp"
#include "RiffLoader.hpp"
#include <sweet/assert/assert.hpp>

using std::map;
using std::string;
using namespace sweet::riff;

FileCache::FileCache()
: files_()
{
}

const File* FileCache::file( const char* identifier ) const
{
    SWEET_ASSERT( identifier );
    map<string, const File*>::const_iterator i = files_.find( string(identifier) );
    return i != files_.end() ? i->second : NULL;
}

void FileCache::swap( FileCache& file_cache )
{
    files_.swap( file_cache.files_ );
}

void FileCache::register_patchers( riff::RiffLoader* loader )
{
    SWEET_ASSERT( loader );
    using std::bind;
    using std::placeholders::_1;
    const char* FILE = "FILE";
    loader->register_patcher( FILE, bind(&FileCache::patch_file, this, _1) );
}

void FileCache::patch_file( const void* data )
{
    SWEET_ASSERT( data );    
    const File* file = reinterpret_cast<const File*>(data);
    files_[string(file->identifier.get())] = file;
}
