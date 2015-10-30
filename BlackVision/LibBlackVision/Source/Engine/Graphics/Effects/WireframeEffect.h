#pragma once

#include "Engine/Graphics/Shaders/RenderableEffect.h"


namespace bv {

class WireframeEffect : public RenderableEffect
{
public:

                        WireframeEffect ();
                        ~WireframeEffect();

private:

    PixelShader *       CreatePS        ();
    VertexShader *      CreateVS        ();
    GeometryShader *    CreateGS        ();

};

} //bv
