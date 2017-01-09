#pragma once

#include "Engine/Graphics/Shaders/RenderableEffect.h"


namespace bv { namespace nrl {


class NFullscreenRenderableEffect : public RenderableEffect
{
public:
    
    NFullscreenRenderableEffect  ( RenderablePass * pass );

};

DEFINE_PTR_TYPE(NFullscreenRenderableEffect)
DEFINE_CONST_PTR_TYPE(NFullscreenRenderableEffect)

} //nrl
} // bv
