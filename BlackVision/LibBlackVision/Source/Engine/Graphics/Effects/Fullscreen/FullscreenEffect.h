#pragma once

#include <vector>

#include "CoreDEF.h"

#include "Engine/Interfaces/IValue.h"

#include "Engine/Graphics/Effects/Fullscreen/FullscreenEffectContext.h"


namespace bv {

enum class FullscreenEffectType : int
{
    FET_SIMPLE_BLIT = 0,
    FET_BLIT_WITH_ALPHA,
    FET_BLIT_WITH_ALPHA_MASK,
    FET_INTERLACE,
    FET_MIX_CHANNELS,
    FET_VIDEO_OUTPUT,
    FET_BLUR,
    //FET_SHADOW
    //FET_OUTLINE
    //TODO: and so on

    FET_TOTAL
};


class FullscreenEffect
{
public:

    virtual                             ~FullscreenEffect       ();

    virtual void                        Render                  ( FullscreenEffectContext * ctx )   = 0;

    virtual unsigned int                GetNumInputs            () const                            = 0;

    virtual void                        SynchronizeInputData    ( FullscreenEffectContext * ctx )   = 0;

    virtual std::vector< IValuePtr >    GetValues               () const                            = 0;

};

DEFINE_PTR_TYPE(FullscreenEffect)
DEFINE_CONST_PTR_TYPE(FullscreenEffect)

} //bv
