#pragma once

#include "Engine/Graphics/Shaders/RenderableEffect.h"


namespace bv {


class FullscreenRenderableEffect : public RenderableEffect
{
public:
    
    FullscreenRenderableEffect  ( RenderablePass * pass );

};

DEFINE_PTR_TYPE(FullscreenRenderableEffect)
DEFINE_CONST_PTR_TYPE(FullscreenRenderableEffect)

} // bv
