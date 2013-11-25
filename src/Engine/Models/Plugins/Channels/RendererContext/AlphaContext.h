#pragma once

#include <glm/glm.hpp>


namespace bv { namespace model {

//FIXME: add different blending modes
class AlphaContext
{
public:

    bool        blendEnabled;   //false
    glm::vec4   blendColor;     //(0,0,0,0)

public:

    AlphaContext();

};

} //model
} //bv
