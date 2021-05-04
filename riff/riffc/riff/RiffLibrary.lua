
function RiffLibrary( riff_library )
    assert( riff_library );
    assert( type(riff_library) == "table" );
    assert( output or riff_library.filename, "No output file specified for riff library" );

    local prefix = prefix or "";
    local tag = riff_library.tag or 'FILE';
    local riff_writer = RiffWriter();
    local file_builder = FileBuilder( riff_writer );
    local errors = 0;

    riff_writer:begin_block( 'RSRC', '' );        
    for _, filename in ipairs(riff_library) do
        assert( filename );
        assert( type(filename) == "string" );
        errors = errors + file_builder:file( ("%s%s"):format(prefix, filename), tag );
    end
    assert( errors == 0, "Building riff library '%s' failed" % {output or riff_library.filename} );
    riff_writer:end_block();
    riff_writer:write_to_file( output or riff_library.filename );
end
