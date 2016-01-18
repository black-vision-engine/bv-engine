#pragma once

#include "Engine/Graphics/Shaders/RenderableEffect.h"


namespace bv {


class FullscreenRenderableEffectTr : public RenderableEffect
{
public:
    
    FullscreenRenderableEffectTr( RenderablePass * pass );

};

DEFINE_PTR_TYPE(FullscreenRenderableEffectTr)
DEFINE_CONST_PTR_TYPE(FullscreenRenderableEffectTr)

} // bv
