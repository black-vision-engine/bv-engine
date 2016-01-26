#pragma once

#include <string>

namespace bv
{

// ******************************
// Implementation of simple hash class with comparation function needed for std::map
class Hash
{
public:

	static Hash					FromString  ( const std::string & str );
    static Hash					FromFile    ( const std::string & path );

	bool						operator <	( const Hash & b ) const;
	bool						operator == ( const Hash & b ) const;

	const std::string &			Get() const;

	explicit					Hash( const std::string & value );

private:
	std::string m_value;
};

} // bv