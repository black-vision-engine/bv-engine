#pragma once

#include <glm/glm.hpp>

#include "Engine/Graphics/State/StateEnums.h"


namespace bv {

class AlphaState
{
public:

    bool                blendEnabled;   // false
    AlphaSrcBlendMode   srcBlendMode;   // ASBM_SRC_ALPHA
    AlphaDstBlendMode   dstBlendMode;   // ADBM_ONE_MINUS_SRC_ALPHA

    bool                compareEnabled; // false
    AlphaCompareMode    compareMode;    // ACM_ALWAYS

    float               alphaReference; // 0, and always in [0,1]
    glm::vec4           blendColor;     // (0,0,0,0)

public:

    AlphaState      ();

};

} //bv
