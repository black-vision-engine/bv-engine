#include "stdafxCore.h"

#include "AABB.h"



#include "Memory/MemoryLeaks.h"



namespace bv { namespace mathematics {

// ******************************
// 
Rect					AABB( const std::vector< glm::vec2 > & points )
{
	Rect result;

	for( auto p : points )
		result.Include( p );

	return result;
}

} // model
} // bv