#include "ResourceDescriptor.h"

namespace bv
{
// ************************
//
bool ResourceDesc::Query( const std::string & uid ) const
{
	return( UID().compare( uid ) == 0 );
}

} // bv