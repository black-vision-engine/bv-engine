#include "Path.h"

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

} // bv