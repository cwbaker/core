#ifndef APPLICATION_HPP_INCLUDED
#define APPLICATION_HPP_INCLUDED

#include <sweet/lua/LuaObject.hpp>
#include <sweet/lua/Lua.hpp>
#include <sweet/io/IoPolicy.hpp>
#include <error/ErrorPolicy.hpp>
#include <string>

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
    sweet::io::IoPolicy io_policy_;
    fs::DirectoryStack* directory_stack_;
    fs::FileSystem* file_system_;
    sweet::lua::Lua lua_;
    fs::LuaFs* lua_fs_;
    sweet::lua::LuaObject* riff_;
    sweet::lua::LuaObject file_builder_metatable_;
    sweet::lua::LuaObject file_builder_prototype_;
    sweet::lua::LuaObject riff_writer_metatable_;
    sweet::lua::LuaObject riff_writer_prototype_;

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

#endif
