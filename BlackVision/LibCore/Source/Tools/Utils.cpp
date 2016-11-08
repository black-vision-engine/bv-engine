#include "stdafx.h"

#include "Utils.h"

namespace bv
{

// ******************************
//
bool		IsPowerOfTwo( UInt32 x )
{
	return (x & (x - 1)) == 0;
}

// ******************************
//
UInt32		RoundUpToPowerOfTwo( UInt32 v )
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;

	return v;
}

// ******************************
//
template<> 
std::string toString< std::string >( const std::string & t )
{
	return t;
}

// *************************************
//
StringVector & split_(const std::string & s, char delim, StringVector & elems) // FIXME: this "_" is so weak
{ 
    std::stringstream ss( s );
    std::string item;

    while ( std::getline( ss, item, delim ) )
    {
        elems.push_back(item);
    }

    return elems;
}

// *************************************
//
StringVector split_ (const std::string & s, char delim )  // FIXME: this "_" is so weak
{
    std::vector< std::string > elems;
    split_( s, delim, elems );
    return elems;
}

} // bv