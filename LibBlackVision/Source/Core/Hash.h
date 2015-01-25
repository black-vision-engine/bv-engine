#pragma once

#include <string>

namespace bv
{

// ******************************
// Implementation of simple hash class with comparation function needed for std::map
class Hash
{
public:

	static Hash					FromString( const std::string & str );

	bool						operator < ( const Hash & b ) const;

private:

	const std::string &			Get() const;

	explicit					Hash( const std::string & value );

	std::string m_value;
};

} // bv