#pragma once

#include "Mathematics/glm_inc.h"

#include "CoreDEF.h"

#include "Engine/Graphics/State/RendererStateEnums.h"


namespace bv {

class AlphaState
{
public:

    bool                blendEnabled;   // false
    AlphaSrcBlendMode   srcRGBBlendMode;   // ASBM_SRC_ALPHA
    AlphaDstBlendMode   dstRGBBlendMode;   // ADBM_ONE_MINUS_SRC_ALPHA
    AlphaSrcBlendMode   srcAlphaBlendMode;   // ASBM_ONE
    AlphaDstBlendMode   dstAlphaBlendMode;   // ADBM_ONE

    glm::vec4           blendColor;     // (0,0,0,0)

public:

    AlphaState      ();

};

DEFINE_PTR_TYPE(AlphaState)
DEFINE_CONST_PTR_TYPE(AlphaState)

} //bv
