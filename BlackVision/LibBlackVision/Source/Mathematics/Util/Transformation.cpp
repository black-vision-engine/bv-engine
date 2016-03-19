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
        auto r = tx *  glm::vec4( v, 1.f );
        ret.Include( glm::vec3( r.x, r.y, r.w ) );
    }
    
    return ret;
}

} // mathematics
} // bv