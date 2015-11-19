#pragma once

#include "Mathematics/glm_inc.h"

#include "Engine/Graphics/Shaders/RenderableEffect.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamInt.h"


namespace bv {

class MaskChannelEffect : public RenderableEffect
{
private:

    ShaderParamInt *  m_maskParam;

public:

                        MaskChannelEffect ();
                        ~MaskChannelEffect();

private:

    PixelShader *       CreatePS        ();
    VertexShader *      CreateVS        ();

};

DEFINE_PTR_TYPE(MaskChannelEffect)
DEFINE_CONST_PTR_TYPE(MaskChannelEffect)

} //bv
