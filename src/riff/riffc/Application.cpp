//
// Application.cpp
// Copyright (c) Charles Baker. All rights reserved.

#include "Application.hpp"
#include <sweet/lua/Lua.hpp>
#include <sweet/lua/LuaObject.hpp>
#include <sweet/lua/LuaDeleter.hpp>
#include <riff/FileBuilder.hpp>
#include <riff/RiffWriter.hpp>
#include <cmdline/Parser.hpp>
#include <fs/fs_lua/LuaFs.hpp>
#include <fs/FileSystem.hpp>
#include <fs/DirectoryStack.hpp>
#include <assert/assert.hpp>
#include <memory>
#include <stdlib.h>

using std::string;
using std::vector;
using std::unique_ptr;
using namespace sweet;
using namespace sweet::lua;
using namespace riff;

SWEET_LUA_TYPE_CONVERSION( FileBuilder, LuaByReference, "riff.FileBuilder" );
SWEET_LUA_TYPE_CONVERSION( RiffWriter, LuaByReference, "riff.RiffWriter" );

Application::Application( int argc, char **argv )
: result_( EXIT_SUCCESS )
, error_policy_()
, io_policy_()
, file_system_( nullptr )
, lua_( error_policy_ )
, lua_fs_( nullptr )
, riff_( nullptr )
, file_builder_metatable_( lua_ )
, file_builder_prototype_( lua_ )
, riff_writer_metatable_( lua_ )
, riff_writer_prototype_( lua_ )
{
    directory_stack_ = new fs::DirectoryStack;
    file_system_ = new fs::FileSystem;
    lua_fs_ = new fs::LuaFs;
    riff_ = new lua::LuaObject( lua_ );

    directory_stack_->reset_directory( file_system_->initial() );
    file_system_->set_directory_stack( directory_stack_ );

    bool help = false;
    bool version = false;
    bool stack_trace = false;
    string require = "";
    string execute = "";
    string root = "";
    string output_filename = "";
    vector<string> assignments_and_filenames;

    cmdline::Parser command_line_parser;
    command_line_parser.add_options()
        ( "help", "h", "Print this message and exit", &help )
        ( "version", "v", "Print the version and exit", &version )
        ( "stack-trace", "s", "Enable stack traces in error messages", &stack_trace )
        ( "require", "r", "Set the name of the script to require on startup", &require )
        ( "execute", "e", "Set the name of the script to execute for each input file", &execute )
        ( "root", "", "Set the root directory", &root )
        ( "output", "o", "Set the output filename", &output_filename )
        ( &assignments_and_filenames )
    ;
    command_line_parser.parse( argc, argv, &error_policy_ );

    if ( version || help || (require.empty() && execute.empty()) || output_filename.empty() )
    {
        if ( version )
        {
            fprintf( stderr, "RIFF Compiler %s\n", BUILD_VERSION );
            fprintf( stderr, "Copyright (c) Charles Baker. All rights reserved. \n" );
        }

        if ( help || (require.empty() && execute.empty()) || output_filename.empty() )
        {
            fprintf( stderr, "Usage: riffc [options] [attribute=value] file ... \n" );
            fprintf( stderr, "Options: \n" );
            command_line_parser.print( stderr );
        }
        
        result_ = version || help ? EXIT_SUCCESS : EXIT_FAILURE;
    }
    else
    {
        lua_.set_stack_trace_enabled( stack_trace );
        prototypes();

        if ( !root.empty() )
        {
            file_system_->set_root( root );
        }
        
        vector<string> filenames;
        for ( std::vector<std::string>::const_iterator i = assignments_and_filenames.begin(); i != assignments_and_filenames.end(); ++i )
        {
            std::string::size_type position = i->find( "=" );
            if ( position != std::string::npos )
            {
                std::string attribute = i->substr( 0, position );
                std::string value = i->substr( position + 1, std::string::npos );
                lua_.globals()
                    ( attribute.c_str(), value )
                ;
            }
            else
            {
                filenames.push_back( *i );
            }
        }

        if ( !require.empty() )
        {
            lua_.call( "require" )( require ).end();            
        }

        if ( !execute.empty() )
        {
            lua_State* lua_state = lua_.get_lua_state();
            lua_newtable( lua_state );
            const int filenames_table = lua_gettop( lua_state );
            for ( int i = 0; i < int(filenames.size()); ++i )
            {
                const string& filename = filenames[i];
                lua_pushlstring( lua_state, filename.c_str(), filename.length() );
                lua_rawseti( lua_state, -2, i + 1 );
            }

            lua_.call( execute.c_str(), execute.c_str() )
                ( output_filename )
                .copy_values_from_stack( filenames_table, filenames_table + 1 )
            .end();
        }
        else
        {
            for ( vector<string>::const_iterator filename = filenames.begin(); filename != filenames.end(); ++filename )
            {
                lua_.call( filename->c_str(), filename->c_str() )
                    ( output_filename )
                    ( filename->c_str() )
                .end();
            }
        }
        
        result_ = error_policy_.errors() == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
    }
}

Application::~Application()
{
    delete riff_;
    delete lua_fs_;
    delete file_system_;
    delete directory_stack_;
}

int Application::result() const
{
    return result_;
}

void Application::prototypes()
{
    file_builder_prototype_.members()
        .type( typeid(FileBuilder) )
        ( "file", &FileBuilder::file )
    ;
    
    file_builder_metatable_.members()
        ( "__index", file_builder_prototype_ )
        ( "__gc", raw(&Application::destroy_file_builder) )
    ;
    
    riff_writer_prototype_.members()
        .type( typeid(RiffWriter) )
        ( "begin_block", &RiffWriter::begin_block )
        ( "end_block", &RiffWriter::end_block )
        ( "write_to_file", &RiffWriter::write_to_file )
    ;
    
    riff_writer_metatable_.members()
        ( "__index", riff_writer_prototype_ )
        ( "__gc", raw(&Application::destroy_riff_writer) )
    ;

    riff_->members()
        ( "FileBuilder", weaken(&Application::create_file_builder), this, lua::_1 )
        ( "RiffWriter", weaken(&Application::create_riff_writer), this )
    ;
    
    lua_.globals()
        ( "riff", riff_ )
        ( "FileBuilder", weaken(&Application::create_file_builder), this, lua::_1 )
        ( "RiffWriter", weaken(&Application::create_riff_writer), this )
    ;

    lua_State* lua_state = lua_.get_lua_state();
    lua_push_object( lua_state, riff_ );
    lua_fs_->create_with_existing_table( file_system_, lua_state );
    lua_pop( lua_state, 1 );
}

FileBuilder* Application::create_file_builder( riff::RiffWriter* riff_writer )
{
    SWEET_ASSERT( riff_writer );
    
    unique_ptr<FileBuilder> file_builder;
    if ( riff_writer )
    {
        file_builder.reset( new FileBuilder(*riff_writer, &error_policy_) );
        lua_.create( file_builder.get() );
        lua_.members( file_builder.get() )
            .metatable( file_builder_metatable_ )
            .this_pointer( file_builder.get() )
        ;
    }
    return file_builder.release();
}

riff::RiffWriter* Application::create_riff_writer()
{
    unique_ptr<riff::RiffWriter> riff_writer( new riff::RiffWriter(&error_policy_) );
    riff_writer->reset();
    lua_.create( riff_writer.get() );
    lua_.members( riff_writer.get() )
        .metatable( riff_writer_metatable_ )
        .this_pointer( riff_writer.get() )
    ;
    return riff_writer.release();
}

int Application::destroy_file_builder( lua_State* lua_state )
{
    const int FILE_BUILDER = 1;
    FileBuilder* file_builder = LuaConverter<FileBuilder*>::to( lua_state, FILE_BUILDER );
    lua_destroy_object( lua_state, file_builder );
    delete file_builder;
    return 0;
}

int Application::destroy_riff_writer( lua_State* lua_state )
{
    const int RESOURCE_WRITER = 1;
    riff::RiffWriter* riff_writer = LuaConverter<riff::RiffWriter*>::to( lua_state, RESOURCE_WRITER );
    lua_destroy_object( lua_state, riff_writer );
    delete riff_writer;
    return 0;
}
