#pragma once

#include <string>

namespace bv
{

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