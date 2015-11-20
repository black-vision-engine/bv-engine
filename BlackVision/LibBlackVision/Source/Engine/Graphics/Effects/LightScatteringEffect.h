#pragma once

#include "Engine/Graphics/Shaders/RenderableEffect.h"


namespace bv {

class LightScatteringEffect : public RenderableEffect
{
public:

                        LightScatteringEffect ();
                        ~LightScatteringEffect();

private:

    PixelShader *       CreatePS        ();
    VertexShader *      CreateVS        ();
    GeometryShader *    CreateGS        ();

};

} //bv
