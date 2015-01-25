#include "DirIO.h"

#include <windows.h>
#include <boost/filesystem.hpp>

#include "Core/StringHeplers.h"

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
        fs::path fullFilePath = rootDir / fs::path( WStringToString( fd.cFileName ) );

        foundFiles.push_back( fullFilePath.string() );
 
        if( FindNextFile( h, &fd ) == FALSE )
            break;
    }

    return foundFiles;
}

// *******************************
//
std::string                  Dir::WorkingDirectory  ()
{
    wchar_t pBuf[ 1024 ];

    int bytes = GetModuleFileName( NULL, pBuf, 1024 );

    if( bytes == 0 )
    {
	    return std::string( "" );
    }
    else
    {
	    return WStringToString( std::wstring( pBuf ) );
    }
}

} //bv
