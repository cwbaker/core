#ifndef FS_SEARCHPATH_HPP_INCLUDED
#define FS_SEARCHPATH_HPP_INCLUDED

#include "BasicPath.ipp"
#include <string>
#include <vector>

namespace fs
{

class SearchPath
{
    std::vector<Path> paths_;

public:
    SearchPath();
    fs::Path find_path_with_file( const std::string& filename ) const;
    fs::Path which( const std::string& filename ) const;
    void clear();
    void append_path( const fs::Path& path );
    void append_path( const std::string& path );
    void append_path( const char* path );
    void append_paths( const std::vector<std::string>& paths );
    void set_paths( const std::vector<std::string>& paths );
};

}

#endif
