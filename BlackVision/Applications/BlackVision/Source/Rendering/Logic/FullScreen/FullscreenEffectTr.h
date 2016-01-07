#pragma once

#include "Rendering/Logic/FullScreen/FullscreenEffectContext.h"


namespace bv {

class FullscreenEffectTr
{
public:

    virtual         ~FullscreenEffectTr();

    virtual void    Render( FullscreenEffectContext * ctx ) = 0;

};

} //bv
