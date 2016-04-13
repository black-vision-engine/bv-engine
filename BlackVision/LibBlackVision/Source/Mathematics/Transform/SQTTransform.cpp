#include "stdafx.h"

#include "Mathematics/Transform/SQTTransform.h"


namespace bv {

// *************************************
//
SQTTransform::SQTTransform()
{
}

// *************************************
//
glm::mat4x4         SQTTransform::Evaluate        ( const glm::vec3 & translation, const glm::quat & rotation, const glm::vec3 & scale, const glm::vec3 & center ) const
{
    glm::mat4x4 ret( 1.0f );

    ret *= glm::translate( glm::mat4( 1.0f ), translation );
    ret *= glm::translate( glm::mat4( 1.0f ), center );
    ret *= glm::mat4_cast( rotation );
    ret *= glm::scale( glm::mat4( 1.0f ), scale );
    ret *= glm::translate( glm::mat4( 1.0f ), -center );

    return ret; 
}

} //bv