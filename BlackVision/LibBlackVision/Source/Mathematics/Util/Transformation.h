#pragma once 

#include "Mathematics/Box.h"

namespace bv { namespace mathematics {

class TransformationUtils
{
public:
     static Box            Transform            ( const Box * box, const glm::mat4 & tx );
     static Box            Project              ( const Box * box, const glm::mat4 & projMatrix );
     static glm::vec2      ToScreenNormCoords   ( const glm::vec2 & screenPos );
};

} // mathematics
} // bv