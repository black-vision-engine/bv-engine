#include "stdafx.h"

#include "Transformation.h"

namespace bv { namespace mathematics {

// ******************************
// 
Box  TransformationUtils::Transform( const Box * box, const glm::mat4 & tx )
{
    auto p0 = glm::vec4( box->xmin, box->ymin, box->zmin, 1.f );
    auto p1 = glm::vec4( box->xmax, box->ymax, box->zmax, 1.f );

    auto p0t = tx * p0;
    auto p1t = tx * p1;

    Box ret;
    ret.Include( glm::vec3( p0t ) );
    ret.Include( glm::vec3( p1t ) );

    return ret;
}

} // mathematics
} // bv