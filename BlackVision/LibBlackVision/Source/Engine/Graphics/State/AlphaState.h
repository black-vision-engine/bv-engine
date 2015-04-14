#pragma once

#include "Mathematics/glm_inc.h"

#include "CoreDEF.h"

#include "Engine/Graphics/State/RendererStateEnums.h"


namespace bv {

class AlphaState
{
public:

    bool                blendEnabled;   // false
    AlphaSrcBlendMode   srcBlendMode;   // ASBM_SRC_ALPHA
    AlphaDstBlendMode   dstBlendMode;   // ADBM_ONE_MINUS_SRC_ALPHA

    glm::vec4           blendColor;     // (0,0,0,0)

public:

    AlphaState      ();

};

DEFINE_PTR_TYPE(AlphaState)
DEFINE_CONST_PTR_TYPE(AlphaState)

} //bv
