#pragma once

#include "Engine/Graphics/Shaders/RenderableEffect.h"


namespace bv {


class FullscreenRenderableEffectOld : public RenderableEffect
{
public:
                                FullscreenRenderableEffectOld   ( RenderablePass * pass );

    static VertexShader *       CreateVS                        ( unsigned int numUVChannels );

private:

    static std::string          GetVSShaderSource               ( unsigned int numUVChannels );

};

DEFINE_PTR_TYPE(FullscreenRenderableEffectOld)
DEFINE_CONST_PTR_TYPE(FullscreenRenderableEffectOld)

} // bv
