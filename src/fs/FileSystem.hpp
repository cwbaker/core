#ifndef FS_FILESYSTEM_HPP_INCLUDED
#define FS_FILESYSTEM_HPP_INCLUDED

#include "BasicPath.ipp"

namespace fs
{

class DirectoryStack;

class FileSystem
{
    fs::Path root_; ///< The full path to the root directory.
    fs::Path initial_; ///< The full path to the initial directory.
    fs::Path executable_; ///< The full path to the build executable directory.
    fs::Path home_; ///< The full path to the user's home directory.
    DirectoryStack* directory_stack_; ///< The stack of directories managed by cd, pushd, popd, etc.

public:
    FileSystem();

    const fs::Path& root() const;
    const fs::Path& initial() const;
    const fs::Path& executable() const;
    const fs::Path& home() const;

    fs::Path root( const fs::Path& path ) const;
    fs::Path initial( const fs::Path& path ) const;
    fs::Path executable( const fs::Path& path ) const;
    fs::Path home( const fs::Path& path ) const;

    std::string root( const char* path ) const;
    std::string initial( const char* path ) const;
    std::string executable( const char* path ) const;
    std::string home( const char* path ) const;

    void set_root( const fs::Path& root );
    void set_initial( const fs::Path& initial );
    void refresh_root();
    void refresh_initial();
    void refresh_executable();
    void refresh_home();

    void set_directory_stack( DirectoryStack* directory_stack );
    DirectoryStack* directory_stack() const;
};

}

#endif
