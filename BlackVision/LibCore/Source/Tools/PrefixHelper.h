#pragma once

#include "CoreDEF.h"


namespace bv {

class PrefixHelper
{
private:

    static const std::string	COPY_PREFIX;
    static const std::string	COPY_REGEX;
    
public:

    static Int32			MatchNames			( const std::string & S, const std::string & T );
    static std::string		PrefixCopy			( Int32 prefixNum );
    static std::string      ReplacePrefix       ( const std::string & s, Int32 prefixNum );

};

}
