//
// BasicPathTraits.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include <fs/fs.hpp>
#include <build.hpp>

using namespace fs;

#if defined(BUILD_OS_WINDOWS)
const char* BasicPathTraits<char>::EMPTY = "";
const char* BasicPathTraits<char>::SEPARATORS = "\\/";
const char* BasicPathTraits<char>::SEPARATOR = "/";
const char* BasicPathTraits<char>::NATIVE_SEPARATOR = "\\";
const char  BasicPathTraits<char>::DRIVE = ':';
const char* BasicPathTraits<char>::CURRENT = ".";
const char* BasicPathTraits<char>::PARENT = "..";
const char  BasicPathTraits<char>::DELIMITER = '.';    

const wchar_t* BasicPathTraits<wchar_t>::EMPTY = L"";
const wchar_t* BasicPathTraits<wchar_t>::SEPARATORS = L"\\/";
const wchar_t* BasicPathTraits<wchar_t>::SEPARATOR = L"/";
const wchar_t* BasicPathTraits<wchar_t>::NATIVE_SEPARATOR = L"\\";
const wchar_t  BasicPathTraits<wchar_t>::DRIVE = L':';
const wchar_t* BasicPathTraits<wchar_t>::CURRENT = L".";
const wchar_t* BasicPathTraits<wchar_t>::PARENT = L"..";
const wchar_t  BasicPathTraits<wchar_t>::DELIMITER = L'.';    

#elif defined(BUILD_OS_LINUX) || defined(BUILD_OS_MACOS)
const char* BasicPathTraits<char>::EMPTY = "";
const char* BasicPathTraits<char>::SEPARATORS = "/";
const char* BasicPathTraits<char>::SEPARATOR = "/";
const char* BasicPathTraits<char>::NATIVE_SEPARATOR = "/";
const char  BasicPathTraits<char>::DRIVE = ':';
const char* BasicPathTraits<char>::CURRENT = ".";
const char* BasicPathTraits<char>::PARENT = "..";
const char  BasicPathTraits<char>::DELIMITER = '.';    

const wchar_t* BasicPathTraits<wchar_t>::EMPTY = L"";
const wchar_t* BasicPathTraits<wchar_t>::SEPARATORS = L"/";
const wchar_t* BasicPathTraits<wchar_t>::SEPARATOR = L"/";
const wchar_t* BasicPathTraits<wchar_t>::NATIVE_SEPARATOR = L"/";
const wchar_t  BasicPathTraits<wchar_t>::DRIVE = L':';
const wchar_t* BasicPathTraits<wchar_t>::CURRENT = L".";
const wchar_t* BasicPathTraits<wchar_t>::PARENT = L"..";
const wchar_t  BasicPathTraits<wchar_t>::DELIMITER = L'.';    

#else
#error "Path traits not defined"
#endif
