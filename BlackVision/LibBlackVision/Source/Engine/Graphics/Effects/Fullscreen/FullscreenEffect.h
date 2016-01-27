#pragma once

#include <vector>

#include "CoreDEF.h"

#include "Engine/Interfaces/IValue.h"

#include "Engine/Graphics/Effects/FullScreen/FullscreenEffectContext.h"


namespace bv {

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
