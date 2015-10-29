#include "Serialization/SerializationHelper.h"

namespace bv { namespace SerializationHelper {

std::vector<std::string> split(const std::string &s, char delim);

template< typename T, typename U >
std::pair< T, U > String2Pair( std::string s )
{
    auto ss = split( s, ',' );
    assert( ss.size() == 2 );
    return std::make_pair< T, U >(
        String2T< T >( ss[ 0 ] ),
        String2T< U >( ss[ 1 ] ) );
};

} }
