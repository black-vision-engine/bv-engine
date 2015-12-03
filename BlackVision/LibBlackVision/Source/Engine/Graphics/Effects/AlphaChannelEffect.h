#pragma once

#include "Mathematics/glm_inc.h"

#include "Engine/Graphics/Shaders/RenderableEffect.h"


namespace bv {

class AlphaChannelEffect : public RenderableEffect
{
public:

                        AlphaChannelEffect	();
                        ~AlphaChannelEffect	();

    void				SetTexture			( Texture2DPtr texture );

private:

    PixelShader *       CreatePS			();
    VertexShader *      CreateVS			();

};

DEFINE_PTR_TYPE(AlphaChannelEffect)
DEFINE_CONST_PTR_TYPE(AlphaChannelEffect)

} //bv
