#include "DirIO.h"

#include <boost/filesystem.hpp>
#include <windows.h>

namespace fs = boost::filesystem;


namespace bv {

// *******************************
//
std::vector< std::wstring > Dir::ListFiles( const std::wstring & path, const std::wstring & wildcardFilter )
{
    fs::path rootDir( path );
    fs::path filePattern( wildcardFilter );

    fs::path fullPath = rootDir / filePattern;

    std::vector< std::wstring > foundFiles;

    WIN32_FIND_DATA fd;
    HANDLE h = FindFirstFile( fullPath.c_str(), &fd );

    if( h == INVALID_HANDLE_VALUE )
    {
        return foundFiles;
    }
 
    while( true )
    {
        foundFiles.push_back( fd.cFileName );
 
        if( FindNextFile( h, &fd ) == FALSE )
            break;
    }

    return foundFiles;
}

} //bv
