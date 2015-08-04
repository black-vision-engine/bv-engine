#pragma once

#include "CoreDEF.h"

#include <sstream>

namespace bv
{

// ******************************
//
bool		IsPowerOfTwo( UInt32 x );

// ******************************
//
UInt32		RoundUpToPowerOfTwo( UInt32 v );


// ******************************
//
template< typename T > 
std::string toString( const T & t )
{
	std::ostringstream ss;
	ss << t;
	return ss.str();
}

// ******************************
//
template<> 
std::string toString< std::string >( const std::string & t );

} // bv