#pragma once

#include "Rendering/Logic/FullScreen/FullscreenEffectContext.h"


namespace bv {

class BaseFullscreenEffect
{
public:

    virtual         ~BaseFullscreenEffect() {}

    virtual void    Render( FullscreenEffectContext * ctx ) = 0;

}; 
} //bv
