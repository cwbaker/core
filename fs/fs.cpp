
#include "fs.hpp"
#include "BasicPath.ipp"

using std::string;

namespace sweet
{
    
namespace fs
{

fs::Path absolute( const fs::Path& path, const fs::Path& base_path )
{
    if ( path.is_absolute() )
    {
        return path;
    }

    fs::Path absolute_path( base_path );
    absolute_path /= path;
    absolute_path.normalize();
    return absolute_path;
}

fs::Path relative( const fs::Path& path, const fs::Path& base_path )
{
    return base_path.relative( path );
}

}

}
