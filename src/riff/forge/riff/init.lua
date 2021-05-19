
local riff = ToolsetPrototype( 'riff' );

function riff.configure( toolset, riff_settings )
    return {
        riffc = riff_settings.riffc or '${bin}/riffc';
        luac = riff_settings.luac or '${bin}/luaxxc';
        lua_path = riff_settings.lua_path or root( 'src/sweet/riff/?.lua' );
        build_riff = riff_settings.build_riff or root( 'src/sweet/riff/riff/build_riff.lua' );
    };        
end

function riff.initialize( toolset )
    toolset.Riff = require( 'forge.riff.Riff' );
    toolset.Luac = require( 'forge.riff.Luac' );
    return toolset;
end

return riff;
