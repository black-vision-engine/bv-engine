#include "DirIO.h"

#include <boost/filesystem.hpp>
#include <windows.h>

namespace fs = boost::filesystem;


namespace bv {

// *******************************
//
std::vector< std::string > Dir::ListFiles( const std::string & path, const std::string & wildcardFilter )
{
    fs::path rootDir( path );
    fs::path filePattern( wildcardFilter );

    fs::path fullPath = rootDir / filePattern;

    std::vector< std::string > foundFiles;

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
