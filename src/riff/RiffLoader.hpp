#ifndef SWEET_RIFF_RIFFLOADER_HPP_INCLUDED
#define SWEET_RIFF_RIFFLOADER_HPP_INCLUDED

#include <map>
#include <string>
#include <functional>
#include <stddef.h>

namespace riff
{

class RiffWriter;
class RiffPatcher;
class Riff;

class RiffLoader
{
    std::map<std::string, std::function<void (const void*)>> patchers_;
    std::map<std::string, const Riff*> riffs_;

public:
    RiffLoader();
    ~RiffLoader();

    const std::function<void (const void*)>* find_patcher( const char* tag ) const;
    const Riff* find_riff( const char* identifier ) const;

    void destroy();
    void swap( RiffLoader& riff_loader );
    void register_patcher( const char* tag, const std::function<void (const void*)>& patcher );   
    const Riff* load_riff_from_file( const char* filename );
    const Riff* load_riff_from_writer( const char* identifier, RiffWriter* writer );
    const Riff* load_riff_from_memory( const char* identifier, unsigned char* data, size_t size );
    void unload_riff( const char* identifier );

private:
    void add_riff( const char* filename, const Riff* riff );
    const Riff* remove_riff( const char* filename );
    void parse_riff( const Riff* riff );
};

}

#endif
