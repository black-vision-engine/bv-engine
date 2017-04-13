#include "stdafxCore.h"

#include "Transformation.h"



#include "Memory/MemoryLeaks.h"



namespace bv { namespace mathematics {

// ******************************
// 
Box             TransformationUtils::Transform( const Box * box, const glm::mat4 & tx )
{
    assert( box->GetVerticies().size() == 8 );

    Box ret;

    for( auto v : box->GetVerticies() )
    {
        auto r = tx *  glm::vec4( v, 1.f );
        ret.Include( glm::vec3( r ) );
    }
    
    return ret;
}

// ******************************
// 
glm::vec3       TransformationUtils::Project            ( const glm::vec3 & p, const glm::mat4 & projMatrix )
{
    auto r = projMatrix *  glm::vec4( p, 1.f );
    return glm::vec3( r / r.w );
}

// ******************************
// 
Box             TransformationUtils::Project            ( const Box * box, const glm::mat4 & projMatrix )
{
    assert( box->GetVerticies().size() == 8 );

    Box ret;

    for( auto v : box->GetVerticies() )
    {
        ret.Include( Project( v, projMatrix ) );
    }
    
    return ret;
}


// ******************************
//
glm::vec2       TransformationUtils::ToScreenNormCoords   ( const glm::vec2 & screenPos )
{
    return glm::vec2( ( screenPos.x + 1.f ) / 2.f, ( screenPos.y + 1.f ) / 2.f );
}

} // mathematics
} // bv