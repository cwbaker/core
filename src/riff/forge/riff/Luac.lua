
local Luac = PatternPrototype( 'Luac' );

function Luac.created( toolset, target )
    target:add_dependency( toolset:SourceFile(toolset.settings.riff.luac) );
    target:add_dependency( toolset:SourceFile(toolset.settings.riff.riffc) );
end

function Luac.build( toolset, target )
    local settings = toolset.settings;
    local architecture = settings.architecture;
    local filename = target:filename();
    local luac = target:dependency( 1 );
    local riffc = target:dependency( 2 );
    local source = target:dependency( 3 );
    local arguments = {
        'luaxxc';
        ('-o "%s.luac"'):format( filename );
        ('"%s"'):format( source );
    };
    printf( leaf(source) );
    system( luac, arguments );

    local root = target.root or settings.root or _G.root();
    local identifier = relative( absolute(source), root );
    local arguments = {
        'riffc';
        ('--root "%s"'):format( root );
        ('-e "%s"'):format( settings.riff.build_riff or root('src/core/src/riff/riffc/riff/build_riff.lua') );
        ('-o "%s"'):format( filename );
        ('identifier=%s'):format( identifier );
        ('"%s.luac"'):format( filename );
    };
    local environment = {
        LUA_PATH = settings.riff.lua_path or root( 'src/core/src/riff/?.lua' );
    };
    system( riffc, arguments, environment, toolset:dependencies_filter(target) );
end

return Luac;
