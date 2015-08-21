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
Path			Path::Join		( int count, ... ) const
{
	va_list args;
    va_start( args, count );

	if( count > 0 )
	{
		Path p = va_arg( args, Path );
		boost::filesystem::path bp( p.Str() );

		for( int i = 1; i < count; ++i )
		{
			Path p = va_arg( args, Path );
			bp /= boost::filesystem::path( p.Str() );
		}

		va_end( args );

		return Path( bp.string() );
	}
	else
	{
		return Path( "" );
	}
}

} // bv