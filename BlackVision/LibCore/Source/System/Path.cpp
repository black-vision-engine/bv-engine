#include "stdafxCore.h"

#pragma warning(disable : 4100)
#pragma warning(disable : 4996)

#include "Path.h"
#include "IO/DirIO.h"
#include "IO/FileIO.h"

#include "UseLoggerLibCoreModule.h"

#pragma warning(push)
#pragma warning(disable : 4100)
#include "boost/filesystem/operations.hpp"
#include "boost/regex.hpp"
#include "boost/algorithm/string.hpp"
#pragma warning(pop)

#include <cstdarg>

#include <sys/stat.h>

// *********************************
// Fragment of source code implemting getting relative path in boost. (http://stackoverflow.com/questions/10167382/boostfilesystem-get-relative-path)
template < >
boost::filesystem::path & boost::filesystem::path::append< typename boost::filesystem::path::iterator >( typename boost::filesystem::path::iterator begin, typename boost::filesystem::path::iterator end, const codecvt_type & )
{ 
    for( ; begin != end ; ++begin )
    {
        *this /= *begin;
    }

    return *this;
}

namespace 
{

// Return path when appended to a_From will resolve to same as a_To
boost::filesystem::path make_relative( boost::filesystem::path a_From, boost::filesystem::path a_To )
{
    a_From = boost::filesystem::absolute( a_From ); 
    a_To = boost::filesystem::absolute( a_To );

    boost::filesystem::path ret;
    boost::filesystem::path::const_iterator itrFrom( a_From.begin() ), itrTo( a_To.begin() );
    // Find common base
    for( boost::filesystem::path::const_iterator toEnd( a_To.end() ), fromEnd( a_From.end() ) ; itrFrom != fromEnd && itrTo != toEnd && *itrFrom == *itrTo; ++itrFrom, ++itrTo );
    // Navigate backwards in directory to reach previously found base
    for( boost::filesystem::path::const_iterator fromEnd( a_From.end() ); itrFrom != fromEnd; ++itrFrom )
    {
        if( (*itrFrom) != "." )
            ret /= "..";
    }
    // Now navigate down the directory branch
    for(auto iter = itrTo; iter != a_To.end(); ++iter)
        ret /= *iter;
    return ret;
}

} // anonymous

// *********************************
// bv namespace begin
namespace bv
{

// *********************************
//
Path::Path()
{}

// *********************************
//
Path::Path( const std::string & str )
{
    boost::filesystem::path p( str );
    m_path = p.normalize().string();
}

// *********************************
//
Path::Path( const char * str )
{
    boost::filesystem::path p( str );
    m_path = p.normalize().string();
}

// *********************************
//
std::string		Path::Str		() const
{
    return m_path;
}

// *********************************
//
Path			Path::Join		( const Path & p ) const
{
    boost::filesystem::path bp( this->Str() );
    bp /= boost::filesystem::path( p.Str() );

    return Path( bp.string() );
}

// *********************************
//
Path			Path::Join		( int count, ... ) const
{
    va_list args;
    va_start( args, count );

    if( count > 0 )
    {
        Path p = va_arg( args, Path );

        for( int i = 1; i < count; ++i )
        {
            p = p.Join( va_arg( args, Path ) );
        }

        va_end( args );

        return p;
    }
    else
    {
        return Path();
    }
}

// *********************************
//
bool			Path::operator <		( const Path & l ) const
{
    return this->Str() < l.Str();
}

// *********************************
//
bool			Path::operator ==		( const Path & l ) const
{
    return this->Str() == l.Str();
}

// *********************************
//
bool			Path::operator !=		( const Path & l ) const
{
    return this->Str() != l.Str();
}

// *********************************
//
Path			Path::operator /		( const Path & p ) const
{
    return this->Join( p );
}

// *********************************
//
bool			Path::Copy				( const Path & from, const Path & to )
{
    boost::system::error_code ec;

    auto parent = boost::filesystem::path( to.Str() ).parent_path();

    if( !Path::Exists( parent.string() ) )
    {
        Dir::CreateDir( parent.string(), true );
    }

    boost::filesystem::copy( boost::filesystem::path( from.Str() ), boost::filesystem::path( to.Str() ), ec );

    if( ec )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Cannot copy: " << ec.message();
        return false;
    }
    else
    {
        return true;
    }
}

// *********************************
//
bool			Path::Remove			( const Path & path )
{
    boost::system::error_code ec;
    boost::filesystem::remove_all( path.Str(), ec );

    if( ec )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Cannot remove: " << ec.message();
        return false;
    }
    else
    {
        return true;
    }
}

// *********************************
//
bool			Path::Rename			( const Path & oldPath, const Path & newPath )
{
    boost::system::error_code ec;
    boost::filesystem::rename( oldPath.Str(), newPath.Str(), ec );

    if( ec )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Cannot remove: " << ec.message();
        return false;
    }
    else
    {
        return true;
    }
}

// *********************************
//
bool			Path::IsFile			( const Path & path )
{
    return boost::filesystem::is_regular_file( path.Str() );
}

// *********************************
//
bool			Path::IsDir				( const Path & path )
{
    return boost::filesystem::is_directory( path.Str() );
}

// *********************************
//
bool			Path::Exists			( const Path & path )
{
    return boost::filesystem::exists( path.Str() );
}

// *********************************
//
PathVec			Path::List				( const Path & path, bool recursive, const std::string exp )
{
    if( Path::Exists( path ) )
    {
        try
        {
            boost::filesystem::path cp( path.Str() ); 
            boost::regex pattern( exp, boost::regex::icase );

            PathVec ret;

            if( IsFile( path ) )
            {
                if( regex_match( path.Str(), pattern ) )
                {
                    ret.push_back( path );
                }

                return ret;
            }

            if( recursive )
            {
                for (	boost::filesystem::recursive_directory_iterator iter( cp ), end;
                        iter != end;
                        ++iter )
                {
                    std::string name = iter->path().filename().string();
                    if ( regex_match( name, pattern ) )
                    {
                        auto p = iter->path();
                        ret.push_back( Path( iter->path().string() ) );
                    }
                }
            }
            else
            {
                for (	boost::filesystem::directory_iterator iter( cp ), end;
                        iter != end;
                        ++iter )
                {
                    std::string name = iter->path().filename().string();
                    if ( regex_match( name, pattern ) )
                    {
                        auto p = iter->path();
                        ret.push_back( Path( iter->path().string() ) );
                    }
                }
            }

            return ret;
        } 
        catch( const std::exception & exc )
        {
            LOG_MESSAGE( SeverityLevel::error ) << "Cannot list files in folder: " << path << " with filter " << exp;
            LOG_MESSAGE( SeverityLevel::error ) << exc.what();
            return PathVec(); 
        }
    }
    else
    {
        return PathVec();
    }
}


// *********************************
//
Path			Path::RelativePath		( const Path & path, const Path & start )
{
    auto rel = make_relative( boost::filesystem::path( start.Str() ), boost::filesystem::path( path.Str() ) );

    return Path( rel.string() );
}

// *********************************
//
bool            Path::IsValisPathName   ( const std::string & path )
{
    Path filePath = Path( path );
    auto dirVec = filePath.ParentPath().Split();// File::GetDirName( path );
    auto fileName = File::GetFileName( path, true );

    bool isWindowsName = boost::filesystem::windows_name( fileName );
    bool isPortableDir = true;

    if( dirVec.size() > 0 && !dirVec[ 0 ].empty() )
    {
        for( auto & dirName : dirVec )
        {
            if( !boost::filesystem::portable_directory_name( dirName ) )
                isPortableDir = false;
        }
    }
    
    return isWindowsName && isPortableDir;
}

// ***********************
//
std::time_t     Path::GetTimestamp      ( const Path & path )
{
    boost::system::error_code error;
    auto timestamp = boost::filesystem::last_write_time( path.Str(), error );
    if( !error )
        return timestamp;
    return 0;
}

// *********************************
//
StringVector    Path::Split				() const
{
    StringVector results;
    boost::split(results, m_path, boost::is_any_of("\\"));

    return results;
}

// *********************************
//
Path            Path::ParentPath        () const
{
    return boost::filesystem::path( m_path ).parent_path().string();
}

// *********************************
//
Path            Path::Absolute          () const
{
    return boost::filesystem::absolute( m_path ).string();
}

// ***********************
//
std::string     Path::GetExtension      () const
{
    return boost::filesystem::path( m_path ).extension().string();
}

std::string     Path::Stem              () const
{
    return boost::filesystem::path( m_path ).stem().string();
}

// *********************************
//
std::ostream & operator << ( std::ostream & os, const Path & p )
{
    return os << p.Str();
}

} // bv
