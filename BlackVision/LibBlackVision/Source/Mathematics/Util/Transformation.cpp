#include "stdafx.h"

#include "Transformation.h"

namespace bv { namespace mathematics {

// ******************************
// 
Box  TransformationUtils::Transform( const Box * box, const glm::mat4 & tx )
{
    assert( box->GetVerticies().size() == 8 );

    Box ret;

    for( auto v : box->GetVerticies() )
    {
        ret.Include( glm::vec3( tx * v ) );
    }
    
    return ret;
}

} // mathematics
} // bv