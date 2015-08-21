#pragma once

#include <string>
#include <vector>



namespace bv
{

class Path
{
public:

						Path			( const std::string & str );

	std::string			Str				() const;

	Path				Join			( const Path & p ) const;
	Path				Join			( int count, /* list of Path instacies */... ) const;

private:
	std::string			m_path;
};

typedef std::vector< Path > PathVec;

} // bv