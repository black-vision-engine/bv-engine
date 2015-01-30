#include "ResourceDescriptor.h"

#include <cassert>

namespace bv
{
namespace {
    int dummy_symbol()
    {
        assert( false );
        return 0;
    }
}
//// ************************
////
//bool ResourceDesc::Query( const std::string & uid ) const
//{
//	return( UID().compare( uid ) == 0 );
//}

} // bv