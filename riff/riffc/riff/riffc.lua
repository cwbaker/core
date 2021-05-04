
-- Provide python like syntax for string interpolation.
getmetatable("").__mod = function( format, args )
    if args then
        if type(args) == "table" then
            return string.format( format, table.unpack(args) );
        else
            return string.format( format, args );
        end
    else
        return format;
    end
end

require 'riff.RiffLibrary';
