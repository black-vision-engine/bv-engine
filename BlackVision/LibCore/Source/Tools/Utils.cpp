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


} // bv