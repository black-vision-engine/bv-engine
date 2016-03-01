#pragma once

#include "Engine/Graphics/Shaders/RenderableEffect.h"


namespace bv {

class BoundingBoxEffect : public RenderableEffect
{
public:

                        BoundingBoxEffect ();
                        ~BoundingBoxEffect();

private:

    PixelShader *       CreatePS        ();
    VertexShader *      CreateVS        ();
    GeometryShader *    CreateGS        ();

};

} //bv
