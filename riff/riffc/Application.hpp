#ifndef APPLICATION_HPP_INCLUDED
#define APPLICATION_HPP_INCLUDED

#include <string>
#include <sweet/lua/LuaObject.hpp>
#include <sweet/lua/Lua.hpp>
#include <sweet/io/IoPolicy.hpp>
#include <sweet/error/ErrorPolicy.hpp>

namespace sweet
{

namespace fs
{

class DirectoryStack;
class FileSystem;
class LuaFs;

}

namespace riff
{

class RiffWriter;
class FileBuilder;

class Application
{
    int result_;
    error::ErrorPolicy error_policy_;
    io::IoPolicy io_policy_;
    fs::DirectoryStack* directory_stack_;
    fs::FileSystem* file_system_;
    lua::Lua lua_;
    fs::LuaFs* lua_fs_;
    lua::LuaObject* riff_;
    lua::LuaObject file_builder_metatable_;
    lua::LuaObject file_builder_prototype_;
    lua::LuaObject riff_writer_metatable_;
    lua::LuaObject riff_writer_prototype_;

public:
    Application( int argc, char** argv );
    ~Application();
    int result() const;

    void prototypes();    
    FileBuilder* create_file_builder( riff::RiffWriter* riff_writer );
    RiffWriter* create_riff_writer();

private:
    static int destroy_file_builder( lua_State* lua_state );
    static int destroy_riff_writer( lua_State* lua_state );
};

}

}

#endif
