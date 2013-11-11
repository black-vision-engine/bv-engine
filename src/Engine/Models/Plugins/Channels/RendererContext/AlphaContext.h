#pragma once

#include <glm/glm.hpp>


namespace bv { namespace model {

//FIXME: add different blending modes
class AlphaContext
{
public:

    bool        blendEnabled;   //false
    bool        testEnabled;    //false
    float       referenceAlpha; //0.f
    glm::vec4   blendColor;     //(0,0,0,0)

public:

    AlphaContext();

};

} //model
} //bv
