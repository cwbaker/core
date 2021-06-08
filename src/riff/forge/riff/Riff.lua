
local Riff = FilePrototype( 'Riff' );

function Riff.created( toolset, target )
    target:add_dependency( toolset:SourceFile(toolset.settings.riff.riffc) );
end

function Riff.build( toolset, target )
    local settings = toolset.settings;
    local filename = target:filename();

    local command_line = {
        'riffc',
        ('--root "%s"'):format( target.root or settings.root or root() );
        ('-e "%s"'):format( settings.riff.build_riff or root('src/core/src/riff/riffc/riff/build_riff.lua') );
        ('-o "%s"'):format( filename ),
        ('tag=%s'):format( target.tag or 'FILE' )
    };

    local riffc = target:dependency( 1 );
    for _, dependency in target:dependencies(2) do 
        if dependency:prototype() ~= toolset.Directory then 
            table.insert( command_line, dependency:filename() );
        end
    end
    
    local environment = {
        LUA_PATH = settings.riff.lua_path or root( 'src/core/src/riff/?.lua' );
    };

    printf( leaf(filename) );
    system( 
        riffc,
        command_line,
        environment,
        toolset:dependencies_filter(target) 
    );
end

return Riff;
