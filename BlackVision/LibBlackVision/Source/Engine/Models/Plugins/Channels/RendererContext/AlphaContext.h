#pragma once

#include "Mathematics/glm_inc.h"


namespace bv { namespace model {

//FIXME: add different blending modes
class AlphaContext
{
public:

    enum class SrcBlendMode : int
    {
        SBM_ZERO = 0,
        SBM_ONE,
        SBM_DST_COLOR,
        SBM_ONE_MINUS_DST_COLOR,
        SBM_SRC_ALPHA,
        SBM_ONE_MINUS_SRC_ALPHA,
        SBM_DST_ALPHA,
        SBM_ONE_MINUS_DST_ALPHA,
        SBM_SRC_ALPHA_SATURATE,
        SBM_CONSTANT_COLOR,
        SBM_ONE_MINUS_CONSTANT_COLOR,
        SBM_CONSTANT_ALPHA,
        SBM_ONE_MINUS_CONSTANT_ALPHA,

        SBM_TOTAL
    };

    enum class DstBlendMode : int
    {
        DBM_ZERO = 0,
        DBM_ONE,
        DBM_SRC_COLOR,
        DBM_ONE_MINUS_SRC_COLOR,
        DBM_SRC_ALPHA,
        DBM_ONE_MINUS_SRC_ALPHA,
        DBM_DST_ALPHA,
        DBM_ONE_MINUS_DST_ALPHA,
        DBM_CONSTANT_COLOR,
        DBM_ONE_MINUS_CONSTANT_COLOR,
        DBM_CONSTANT_ALPHA,
        DBM_ONE_MINUS_CONSTANT_ALPHA,

        DBM_TOTAL
    };

public:

    bool            blendEnabled;   //false
    glm::vec4       blendColor;     //(0,0,0,0)

    SrcBlendMode    srcRGBBlendMode;   //SBM_SRC_ALPHA
    DstBlendMode    dstRGBBlendMode;   //DBM_ONE_MINUS_SRC_ALPHA

    SrcBlendMode    srcAlphaBlendMode;   //SBM_ONE
    DstBlendMode    dstAlphaBlendMode;   //DBM_ONE

public:

							AlphaContext	();

    AlphaContext *			Clone			() const;
};

} //model
} //bv
