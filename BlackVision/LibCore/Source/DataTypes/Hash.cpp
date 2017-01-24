#include "stdafxCore.h"

#include "Hash.h"


#pragma warning(push)
#pragma warning(disable : 4512) // assignment operator could not be generated
#pragma warning(disable : 4267) // conversion from 'size_t' to 'type', possible loss of data
#pragma warning(disable : 4996) // fopen is deprecated, consider using fopen_s instead
#pragma warning(disable : 4706) // assignment within conditional expression

#include "hashlibpp.h"

#include <map>
#include <cassert>

#pragma warning(pop)

namespace bv {

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
Hash						Hash::FromFile( const std::string & path )
{
	auto md5 = md5wrapper();

    auto res = Hash( md5.getHashFromFile( path ) );

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