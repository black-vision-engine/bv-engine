#include "DirIO.h"

#include <windows.h>
#include <boost/filesystem.hpp>

#include "Tools/StringHeplers.h"

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
 
    for(;;)
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

// *******************************
//
bool						Dir::Exists				( const std::string & path )
{
	auto ftyp = GetFileAttributesA( path.c_str() );
	
	if ( ftyp == INVALID_FILE_ATTRIBUTES )
	{
		return false;
	}

	if ( ftyp & FILE_ATTRIBUTE_DIRECTORY )
	{
		return true;
	}

	return false;
}

} //bv
