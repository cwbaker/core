#ifndef SWEET_FSYS_LUAFSYS_HPP_INCLUDED
#define SWEET_FSYS_LUAFSYS_HPP_INCLUDED

#include <sweet/fs/BasicPath.hpp>

struct lua_State;

namespace sweet
{
    
namespace fs
{

class FileSystem;

class LuaFs
{
    lua_State* lua_state_;

public:
    LuaFs();
    ~LuaFs();
    void create( FileSystem* file_system, lua_State* lua_state );
    void create_with_existing_table( FileSystem* file_system, lua_State* lua_state );
    void destroy();

private:
    static int cd( lua_State* lua_state );
    static int pushd( lua_State* lua_state );
    static int popd( lua_State* lua_state );
    static int pwd( lua_State* lua_state );
    static int absolute( lua_State* lua_state );
    static int relative( lua_State* lua_state );
    static int root( lua_State* lua_state );
    static int initial( lua_State* lua_state );
    static int executable( lua_State* lua_state );
    static int home( lua_State* lua_state );
}; 

}

}

#endif
