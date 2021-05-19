#ifndef RIFF_LUARIFF_HPP_INCLUDED
#define RIFF_LUARIFF_HPP_INCLUDED

#include <stdint.h>

struct lua_State;

namespace riff
{

class FileCache;

class LuaRiff
{
    FileCache* file_cache_;
    lua_State* lua_state_;

public:
    LuaRiff();
    ~LuaRiff();
    void create( const char* path, FileCache* file_cache, lua_State* lua_state );
    void destroy();
    void refresh();

private:
    static int lua_loader( lua_State* lua_state );
    static int lua_searcher( lua_State* lua_state );
    static int lua_searchpath( lua_State* lua_state );
    static int lua_dofilecont( lua_State* lua_state, int /*status*/, intptr_t /*context*/ );
    static int lua_dofile( lua_State* lua_state );
    static int lua_loadfile( lua_State* lua_state );
};
    
} 

#endif
