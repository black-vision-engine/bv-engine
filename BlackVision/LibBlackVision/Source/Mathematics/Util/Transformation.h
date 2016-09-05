#pragma once 

#include "Mathematics/Box.h"

namespace bv { namespace mathematics {

class TransformationUtils
{
public:

    // ******************************
    // A silmple transformation of a box in a 3 demintional space.
    static Box            Transform            ( const Box * box, const glm::mat4 & tx );

    // ******************************
    // A projection of a box onto a projection plane according to a projection matrix 'projMatrix'
    static Box            Project              ( const Box * box, const glm::mat4 & projMatrix );

    // ******************************
    // A projection of a box onto a projection plane according to a projection matrix 'projMatrix'
    static glm::vec3      Project              ( const glm::vec3 & p, const glm::mat4 & projMatrix );

    // ******************************
    // A trasfomation of a projected point into a space ([0,1]x[0,1]) of displaing texture.
    static glm::vec2      ToScreenNormCoords   ( const glm::vec2 & screenPos );
};

} // mathematics
} // bv