
local function printf( format, ... )
	io.write( format:format(...) );
end

local output_filename = select( 1, ... );
local filenames = select( 2, ... );
local tag = _G.tag or 'FILE';
local riff_writer = riff.RiffWriter();
local file_builder = riff.FileBuilder( riff_writer );
local identifier = _G.identifier or nil;
local errors = 0;

assert( not identifier or #filenames == 1, 'identifier specified with more than one input file' );

riff_writer:begin_block( 'RESOURCE', '' );
for _, filename in ipairs(filenames) do
	local filename = riff.absolute( filename );
	local identifier = identifier or riff.relative( filename, riff.root() );
    errors = errors + file_builder:file( identifier, tag, filename );
end
assert( errors == 0 );
riff_writer:end_block();
riff_writer:write_to_file( output_filename );
