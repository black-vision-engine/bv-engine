#pragma once

#include "CoreDEF.h"

#include <string>
#include <vector>
#include <iterator>

namespace bv
{

class Path
{
public:
	typedef std::vector< Path > PathVec;

						Path			();
						Path			( const std::string & str );
						Path			( const char * str );

	std::string			Str				() const;

	Path				Join			( const Path & p ) const;
	Path				Join			( int count, /* list of Path instancies */... ) const;

	Path				operator /		( const Path & p ) const;

	bool				operator <		( const Path & b ) const; // used by std::set
	bool				operator ==		( const Path & b ) const; // used by std::set

	StringVector        Split			() const;

	static bool			Copy			( const Path & from, const Path & to );
	static bool			Remove			( const Path & path );
	static bool			Rename			( const Path & oldPath, const Path & newPath );
	static bool			IsFile			( const Path & path );
	static bool			IsDir			( const Path & path );
	static bool			Exists			( const Path & path );
	static PathVec		List			( const Path & path, const std::string exp = "*" );
	static Path			RelativePath	( const Path & path, const Path & start );

private:
	std::string			m_path;

};

std::ostream & operator << ( std::ostream & os, const Path & p );

typedef Path::PathVec PathVec;

} // bv