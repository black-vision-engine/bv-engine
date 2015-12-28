#include "PrefixHelper.h"

#include <regex>

namespace bv {

const std::string	PrefixHelper::COPY_PREFIX	=		"Copy#_";
const std::string	PrefixHelper::COPY_REGEX	=		"^(Copy([0-9]*)_)?";

// *******************************
//
Int32					PrefixHelper::MatchNames		( const std::string & srcName, const std::string & destName )
{
	Int32 num = -1;
	auto nameWithoutPrefix = std::regex_replace( srcName, std::regex( COPY_REGEX ), "" );

	std::smatch sm;
	if( std::regex_match( destName, sm, std::regex( COPY_REGEX + nameWithoutPrefix  + "$" ) ) )
	{
		if( sm[ 2 ].matched )
		{
			if( sm[ 2 ].str().empty() )
			{
				num = 0;
			}
			else
			{
				auto ires = ( Int32 )std::stoi( sm[ 2 ].str() );
				if( num < ires )
				{
					num = ires;
				}
			}
		}
	}

	return num;
}

// *******************************
//
std::string				PrefixHelper::PrefixCopy			( Int32 prefixNum )
{
	if( prefixNum == 0 )
	{
		std::string prefix( COPY_PREFIX );
		prefix.replace( prefix.find( "#" ), 1, "" );
		return prefix;
	}

	std::string prefix( COPY_PREFIX );
	prefix.replace( prefix.find( "#" ), 1, std::to_string( prefixNum ) );
	return prefix;
}

// *******************************
//
std::string      PrefixHelper::ReplacePrefix       ( const std::string & s, Int32 prefixNum )
{
    return std::regex_replace( s, std::regex( COPY_REGEX ), PrefixCopy( prefixNum ) );
}

}	
