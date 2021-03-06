#ifndef FS_BASICPATHTRAITS_HPP_INCLUDED
#define FS_BASICPATHTRAITS_HPP_INCLUDED

namespace fs
{

/**
// Provide BasicPaths with trait information.
*/
template <class Char>
struct BasicPathTraits
{
};

/**
// Provide BasicPaths with trait information for strings based on chars.
*/
template <> struct BasicPathTraits<char>
{
    typedef char char_type;

    static const char* EMPTY;
    static const char* SEPARATORS;
    static const char* SEPARATOR;
    static const char* NATIVE_SEPARATOR;
    static const char DRIVE;
    static const char* CURRENT;
    static const char* PARENT;
    static const char DELIMITER;
};


/**
// Provide BasicPaths with trait information for strings based on wchar_ts.
*/
template <> struct BasicPathTraits<wchar_t>
{
    typedef wchar_t char_type;

    static const wchar_t* EMPTY;
    static const wchar_t* SEPARATORS;
    static const wchar_t* SEPARATOR;
    static const wchar_t* NATIVE_SEPARATOR;
    static const wchar_t DRIVE;
    static const wchar_t* CURRENT;
    static const wchar_t* PARENT;
    static const wchar_t DELIMITER;    
};

}

#endif
