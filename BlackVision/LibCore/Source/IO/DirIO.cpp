#include "DirIO.h"

#include "Tools/StringHeplers.h"

#include <boost/filesystem.hpp>

#include <iostream>
#include "win_sock.h"


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

// *******************************
//
bool						Dir::CreateDir			( const std::string & path, bool createRecusive )
{
	if( Exists( path ) )
	{
		return true;
	}

    boost::system::error_code ec;
	boost::filesystem::path p( path );


	if( createRecusive )
	{
		boost::filesystem::path currPath("");
		for( auto it : p )
		{
			currPath += it;
			currPath += "/";
			boost::filesystem::create_directory( currPath, ec );
			if( ec )
			{
				std::cout << "[File::CreateDir] create_directory error: " << ec << std::endl;
				return false;
			}
		}

		return true;
	}
	else
	{
		boost::filesystem::create_directory( path, ec );
		if( ec )
		{
			std::cout << "[File::CreateDir] create_directory error: " << ec << std::endl;
			return false;
		}
		else
		{
			return true;
		}
	}
}

} //bv
