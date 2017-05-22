#include "stdafxCore.h"

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
        auto fileName = WStringToString( fd.cFileName ).ham;

        if( fileName != "." && fileName != ".." )
        {
            fs::path fullFilePath = rootDir / fs::path( fileName );
            foundFiles.push_back( fullFilePath.string() );
        }

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
bool                        Dir::Exists                ( const std::string & path )
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
bool                        Dir::CreateDir            ( const std::string & path, bool createRecusive )
{
    if( Exists( path ) )
    {
        return true;
    }

    boost::system::error_code ec;
    boost::filesystem::path p( path );


    if( createRecusive )
    {    
        boost::filesystem::create_directories( path, ec );

        if( ec )
        {
            LOG_MESSAGE( SeverityLevel::error ) << "[File::CreateDir] create_directory error: " << ec;
            return false;
        }

        return true;
    }
    else
    {
        boost::filesystem::create_directory( path, ec );
        if( ec )
        {
            LOG_MESSAGE( SeverityLevel::error ) << "[File::CreateDir] create_directory error: " << ec;
            return false;
        }
        else
        {
            return true;
        }
    }
}

// *******************************
//
bool                    Dir::RemoveDir          ( const std::string & path )
{
    if( Exists( path ) )
    {
        boost::system::error_code ec;
        boost::filesystem::path p( path );

        boost::filesystem::remove_all( path, ec );

        if( ec )
        {
            LOG_MESSAGE( SeverityLevel::error ) << "[File::CreateDir] create_directory error: " << ec;
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}

// *******************************
//
bool                    Dir::RenameDir          ( const std::string & path,const std::string & newName )
{
    if( Exists( path ) )
    {
        boost::system::error_code ec;
        boost::filesystem::path p( path );


        boost::filesystem::rename( path,newName, ec );

        if( ec )
        {
            LOG_MESSAGE( SeverityLevel::error ) << "[File::CreateDir] create_directory error: " << ec;
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}

// *******************************
//
bool                    Dir::CopyDir            ( const std::string & path, const std::string & newName )
{
    if( Exists( path ) )
    {
        boost::system::error_code ec;
        boost::filesystem::path p( path );


        boost::filesystem::copy_directory( path, newName, ec );

        if( ec )
        {
            LOG_MESSAGE( SeverityLevel::error ) << "[File::CopyDir] copy_directory error: " << ec;
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}

// *******************************
//
UInt64                  Dir::GetSize            ( const std::string & path )
{
    UInt64 size = 0;

    for(    boost::filesystem::recursive_directory_iterator it( path );
            it != boost::filesystem::recursive_directory_iterator();
            ++it )
    {   
        if( !is_directory( *it ) )
        {
            size += boost::filesystem::file_size( *it );
        }
    }

    return size;
}

} //bv
