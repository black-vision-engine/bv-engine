#pragma once

#include "CoreDEF.h"

#include <string>
#include <vector>
#include <iterator>
#include <ctime>


namespace bv {

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
    bool				operator !=		( const Path & b ) const;

	StringVector        Split			() const;
    Path                ParentPath      () const;
    Path                Absolute        () const;

    std::string         GetExtension    () const;
    std::string         Stem            () const;

	static bool			Copy			( const Path & from, const Path & to );
	static bool			Remove			( const Path & path );
	static bool			Rename			( const Path & oldPath, const Path & newPath );
	static bool			IsFile			( const Path & path );
	static bool			IsDir			( const Path & path );
	static bool			Exists			( const Path & path );
	static PathVec		List			( const Path & path, bool recursive, const std::string exp = ".*" );
	static Path			RelativePath	( const Path & path, const Path & start );
    static bool         IsValisPathName ( const std::string & path );

    static std::time_t  GetTimestamp    ( const Path & path );

private:

	std::string			m_path;

};

std::ostream & operator << ( std::ostream & os, const Path & p );

typedef Path::PathVec PathVec;

} // bv
