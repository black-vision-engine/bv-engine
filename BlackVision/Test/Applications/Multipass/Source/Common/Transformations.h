#pragma once

#include "Common/PrototyperCore.h"

#include "Engine/Graphics/Types/Transform.h"


namespace bv {

class Transformations
{
public:

    static Transform    Rotation    ( const glm::vec3 & axis, float angle );
    static Transform    Translation ( const glm::vec3 & v );
    static Transform    Scale       ( const glm::vec3 & s );

};

} //bv
