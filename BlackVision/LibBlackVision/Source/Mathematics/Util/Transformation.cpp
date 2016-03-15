#include "stdafx.h"

#include "Transformation.h"

namespace bv { namespace mathematics {

// ******************************
// 
Box  TransformationUtils::Transform( const Box * box, const glm::mat4 & tx )
{
    auto p0 = glm::vec4( box->xmin, box->ymin, box->zmin, 1.f );
    auto p1 = glm::vec4( box->xmax, box->ymax, box->zmax, 1.f );

    auto p0t = p0 * tx;
    auto p1t = p1 * tx;

    Box ret;
    ret.Include( glm::vec3( p0 ) );
    ret.Include( glm::vec3( p1 ) );

    return ret;
}

} // mathematics
} // bv