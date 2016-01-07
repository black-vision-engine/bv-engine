#pragma once

#include "CoreDEF.h"

#include "Rendering/Logic/FullScreen/FullscreenEffectContext.h"


namespace bv {

class FullscreenEffectTr
{
public:

    virtual         ~FullscreenEffectTr();

    virtual void    Render( FullscreenEffectContext * ctx ) = 0;

};

DEFINE_PTR_TYPE(FullscreenEffectTr)
DEFINE_CONST_PTR_TYPE(FullscreenEffectTr)

} //bv
