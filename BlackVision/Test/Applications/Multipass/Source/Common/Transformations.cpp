#include "Transformations.h"

namespace  bv {

// ************************
//
Transform    Transformations::Rotation    ( const glm::vec3 & axis, float angle )
{
    return Transform( glm::rotate( glm::mat4( 1.0f ), angle, axis ) );
}

// ************************
//
Transform    Transformations::Translation ( const glm::vec3 & v )
{
    return Transform( glm::translate( glm::mat4( 1.0f ), v ) );
}

// ************************
//
Transform    Transformations::Scale       ( const glm::vec3 & s )
{
    return Transform( glm::scale( glm::mat4( 1.0f ), s ) );
}

} // bv
