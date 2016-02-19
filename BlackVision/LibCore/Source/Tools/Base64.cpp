#pragma warning(push)
#pragma warning(disable : 4996)
#pragma warning(disable : 4127)

#include "Base64.h"

#include "boost/archive/iterators/base64_from_binary.hpp"
#include "boost/archive/iterators/binary_from_base64.hpp"
#include "boost/archive/iterators/transform_width.hpp"
#include "boost/archive/iterators/ostream_iterator.hpp"

#include <sstream>
#include <string>

#pragma warning(pop)

namespace bv
{

//typedefs
typedef boost::archive::iterators::base64_from_binary<
          boost::archive::iterators::transform_width< const char *, 6, 8>
> bin_to_base64;

typedef boost::archive::iterators::transform_width<
    boost::archive::iterators::binary_from_base64< const char * >, 8, 6
> base64_to_bin;

// ******************************
//
std::string EncodeBase64( MemoryChunkConstPtr data )
{
    using namespace boost::archive::iterators;

    std::stringstream os;

    auto padding = data->Size() % 3;

    if( padding > 0 )
    {
        padding = 3 - padding;
    }

    std::copy(
        bin_to_base64( data->Get() ),
        bin_to_base64( data->Get() + data->Size() ),
        ostream_iterator< char >( os )
    );

    auto res = os.str();

    for( auto i = 0; i < padding; ++i )
    {
        res.append( "=" );
    }

    return res;
}

// ******************************
//
MemoryChunkConstPtr DecodeBase64( const std::string & enc )
{ 
    using namespace boost::archive::iterators;

    auto l = enc.find_first_of( "=" );

    auto d = enc.size();

    SizeType padding = 0;

    if( l != std::string::npos )
    {
        padding = d - l;
    }
    
    d -= padding;    

    char * buf = new char[ enc.size() * 3 / 4 ];

    std::copy(
        base64_to_bin( enc.c_str() ),
        base64_to_bin( enc.c_str() + d ),
        buf
    );

    return MemoryChunk::Create( buf,  enc.size() * 3 / 4 - padding );
}

} // bv
