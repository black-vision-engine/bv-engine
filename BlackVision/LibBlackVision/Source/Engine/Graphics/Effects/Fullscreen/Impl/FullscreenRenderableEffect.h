#pragma once

#include "Engine/Graphics/Shaders/RenderableEffect.h"


namespace bv {


class FullscreenRenderableEffect : public RenderableEffect
{
public:
                                FullscreenRenderableEffect  ( RenderablePass * pass );

    static VertexShader *       CreateVS                    ( unsigned int numUVChannels );

private:

    static std::string          GetVSShaderSource           ( unsigned int numUVChannels );

};

DEFINE_PTR_TYPE(FullscreenRenderableEffect)
DEFINE_CONST_PTR_TYPE(FullscreenRenderableEffect)

} // bv
