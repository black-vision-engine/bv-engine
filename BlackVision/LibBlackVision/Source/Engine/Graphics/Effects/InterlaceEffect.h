#pragma once

#include "Mathematics/glm_inc.h"

#include "Engine/Graphics/Shaders/RenderableEffect.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamInt.h"


namespace bv {

class InterlaceEffect : public RenderableEffect
{
private:

public:

                        InterlaceEffect ( Texture2DPtr tex0, Texture2DPtr tex1 );
                        ~InterlaceEffect();

private:

    PixelShader *       CreatePS        ();
    VertexShader *      CreateVS        ();

};

DEFINE_PTR_TYPE(InterlaceEffect)
DEFINE_CONST_PTR_TYPE(InterlaceEffect)

} //bv
