#include "Hash.h"


#pragma warning(push)
#pragma warning(disable : 4512)
#pragma warning(disable : 4267)

#include "hashlibpp.h"

#include <map>
#include <cassert>

#pragma warning(pop)

namespace bv
{

// ******************************
//
const std::string &			Hash::Get() const
{
	return m_value;
}

// ******************************
//
Hash						Hash::FromString( const std::string & str )
{
	auto md5 = md5wrapper();
	auto res = Hash( md5.getHashFromString( str ) );

	return res;
}

// ******************************
//
Hash::Hash( const std::string & value )
	: m_value( value )
{}

// ******************************
//
bool						Hash::operator < ( const Hash & b ) const
{
	return m_value.compare( b.m_value ) < 0;
}

// ******************************
//
bool						Hash::operator == ( const Hash & b ) const
{
	return m_value == b.m_value;
}


} // bv