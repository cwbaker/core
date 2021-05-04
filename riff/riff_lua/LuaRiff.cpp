//
// LuaRiff.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "LuaRiff.hpp"
#include <sweet/riff/FileCache.hpp>
#include <sweet/riff/File.hpp>
#include <sweet/assert/assert.hpp>
#include <lua.hpp>
#include <string.h>

using namespace sweet::riff;

LuaRiff::LuaRiff()
: file_cache_( nullptr ),
  lua_state_( nullptr )
{
}

LuaRiff::~LuaRiff()
{
    destroy();
}

void LuaRiff::create( const char* /*path*/, FileCache* file_cache, lua_State* lua_state )
{
    SWEET_ASSERT( file_cache );
    SWEET_ASSERT( lua_state );
    destroy();

    file_cache_ = file_cache;
    lua_state_ = lua_state;

    static const luaL_Reg functions[] = 
    {
        { "loader", lua_loader },
        { "searcher", lua_searcher },
        { "searchpath", lua_searchpath },
        { "dofile", lua_dofile },
        { "loadfile", lua_loadfile },
        { "loaded", nullptr },
        { nullptr, nullptr }
    };

    luaL_newlibtable( lua_state, functions );
    lua_pushlightuserdata( lua_state, this );
    luaL_setfuncs( lua_state, functions, 1 );

    // Set loaded field to an empty table.
    lua_newtable( lua_state );
    lua_setfield( lua_state, -2, "loaded" );

    // Set the global variable 'riff' to the module table.
    lua_setglobal( lua_state, "riff" );
}

void LuaRiff::destroy()
{
    file_cache_ = nullptr;
    lua_state_ = nullptr;
}

void LuaRiff::refresh()
{
    SWEET_ASSERT( lua_state_ );

    if ( lua_state_ )
    {
        // Get `package.loaded` to reset fields in.
        lua_getglobal( lua_state_, "package" );
        lua_getfield( lua_state_, -1, "loaded" );

        // Get `riff.loaded` to get fields to reset from.
        lua_getglobal( lua_state_, "riff" );
        lua_getfield( lua_state_, -1, "loaded" );

        lua_pushnil( lua_state_ );
        while ( lua_next(lua_state_, -2) != 0 ) 
        {
            // Pop value and set `package.loaded[key] = nil`.
            lua_pop( lua_state_, 1 );
            lua_pushvalue( lua_state_, -1 );
            lua_pushnil( lua_state_ );
            lua_settable( lua_state_, -6 );
        }

        // Set `riff.loaded = {}`.
        lua_newtable( lua_state_ );
        lua_setfield( lua_state_, -4, "loaded" );

        // Restore Lua stack.
        lua_pop( lua_state_, 5 );
    }
}

int LuaRiff::lua_loader( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int IDENTIFIER = 1;
    const int FILE = 2;

    const char* identifier = luaL_optstring( lua_state, IDENTIFIER, nullptr );
    const File* file = reinterpret_cast<File*>( lua_touserdata(lua_state, FILE) );

    if ( file && luaL_loadbufferx(lua_state, file->data.get(), file->length, file->filename.get(), "b") == LUA_OK )
    {
        // Call the loaded chunk passing the module identifier.
        lua_pushvalue( lua_state, IDENTIFIER );
        lua_call( lua_state, 1, 1 );

        // Set `riff.loaded[identifier]` to true so that it can be used 
        // to clear out the entry in `package.loaded()` on refresh.
        lua_getglobal( lua_state, "riff" );
        lua_getfield( lua_state, -1, "loaded" );
        lua_pushvalue( lua_state, IDENTIFIER );
        lua_pushboolean( lua_state, 1 );
        lua_settable( lua_state, -3 );
        lua_pop( lua_state, 2 );

        return 1;
    }

    if ( !file )
    {
        lua_pushnil( lua_state );
        lua_pushfstring( lua_state, "no riff provided loading '%s'", identifier );
        return 2;
    }

    // An error has occured loading the Lua chunk from the riff and the
    // error message is at the top of the stack so push nil, insert the error
    // message back on top of the stack and return both values.
    lua_pushnil( lua_state );
    lua_insert( lua_state, -2 );
    return 2;
}

int LuaRiff::lua_searcher( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );
    lua_getglobal( lua_state, "package" );
    lua_getfield( lua_state, -1, "path" );
    const char* path = lua_tostring( lua_state, -1 );
    if ( !path )
    {
        return luaL_error( lua_state, "'riff.path' must be a string" );
    }

    lua_remove( lua_state, -2 );
    lua_searchpath( lua_state );
    if ( lua_isnil(lua_state, -2) )
    {
        lua_remove( lua_state, -2 );        
        return 1;
    }

    lua_pushcfunction( lua_state, lua_loader );
    lua_insert( lua_state, -2 );
    return 2;
}

/**
// Implement a Lua searcher that searches through patched Lua riffs.
//
// See the Lua manual documentation for `package.searchers` and 
// `package.searchpath()` upon which this function is based.
//
// @param lua_state
//  The Lua virtual machine that `riff.searchpath()` has been called from.
//
// @return
//  The number of return values pushed (1 filename on success, 2 nil and an 
//  error message on failure).
*/
int LuaRiff::lua_searchpath( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const char PATH_SEPARATOR = ';';
    const char* PATH_MARK = "?";

    luaL_Buffer error_message;
    luaL_buffinit( lua_state, &error_message );

    LuaRiff* riff = reinterpret_cast<LuaRiff*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( riff );

    // Replace occurences of "." with "/" so that the convention of using
    // dots to locate modules within subdirectories works when searching the
    // riff identifier namespace.
    const char* module = luaL_checkstring( lua_state, 1 );
    const char* path = luaL_checkstring( lua_state, 2 );
    module = luaL_gsub( lua_state, module, ".", "/" );

    int start = 0;
    int finish = start + 1;
    int end = strlen( path );
    while ( start < end )
    {
        // Find the start of the next template
        while ( path[start] == PATH_SEPARATOR )
        {
            ++start;
        }

        // Find the finish of the next template
        const char* next_separator = strchr( path + start, PATH_SEPARATOR );
        if ( !next_separator )
        {
            next_separator = path + strlen( path );
        }
        finish = next_separator - path;

        // Push the template onto the Lua stack with all occurrences of the
        // path mark "?" replaced by the module name.
        lua_pushlstring( lua_state, path + start, finish - start );
        const char* identifier = luaL_gsub( lua_state, lua_tostring(lua_state, -1), PATH_MARK, module );
        lua_remove( lua_state, 2 );

        const riff::File* file = riff->file_cache_->file( identifier );
        if ( file )
        {
            lua_pushlightuserdata( lua_state, const_cast<riff::File*>(file) );
            return 2;
        }

        lua_pushfstring( lua_state, "\n\tno riff '%s'", identifier );
        lua_remove( lua_state, -2 );
        luaL_addvalue( &error_message );
        
        start = finish;
    }
    luaL_pushresult( &error_message );
    lua_pushnil( lua_state );
    lua_insert( lua_state, -2 );
    return 2;
}

int LuaRiff::lua_dofilecont( lua_State* lua_state, int /*status*/, lua_KContext /*context*/ ) 
{
    return lua_gettop( lua_state ) - 1;
}

int LuaRiff::lua_dofile( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );    
    lua_settop( lua_state, 1 );
    int result = lua_loadfile( lua_state );
    if ( result == 1 )
    {
        lua_callk( lua_state, 0, LUA_MULTRET, 0, lua_dofilecont );
        return lua_dofilecont( lua_state, 0, 0 );
    }
    SWEET_ASSERT( result == 2 );
    return result;
}

int LuaRiff::lua_loadfile( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int IDENTIFIER = 1;
    const int MODE = 2;
    const int ENVIRONMENT = 3;

    LuaRiff* riff = reinterpret_cast<LuaRiff*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    const char* identifier = luaL_optstring( lua_state, IDENTIFIER, nullptr );
    const char* mode = luaL_optstring( lua_state, MODE, nullptr );

    const File* file = riff->file_cache_->file( identifier );
    if ( file && luaL_loadbufferx(lua_state, file->data.get(), file->length, file->filename.get(), mode) == LUA_OK)
    {
        if ( !lua_isnone(lua_state, ENVIRONMENT) ) 
        {
            lua_pushvalue( lua_state, ENVIRONMENT );
            if ( !lua_setupvalue(lua_state, -2, 1) )
            {
                lua_pop( lua_state, 1 );
            }
        }
        return 1;
    }

    // An error has occured and the error message is at the top of the stack
    // so push nil, insert the error message back on top of the stack and 
    // return both values.
    lua_pushnil( lua_state );
    lua_insert( lua_state, -2 );
    return 2;
}
