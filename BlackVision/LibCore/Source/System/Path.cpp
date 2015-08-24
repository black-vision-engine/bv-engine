#include "Path.h"

#include "Tools/Logger/Logger.h"
#define LOG_MODULE ModuleEnum::ME_LibCore

#include "boost/filesystem/operations.hpp"

#include <cstdarg>

namespace bv
{

// *********************************
//
Path::Path( const std::string & str )
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
	boost::filesystem::path bp( p.Str() );
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
		return Path( "" );
	}
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

} // bv