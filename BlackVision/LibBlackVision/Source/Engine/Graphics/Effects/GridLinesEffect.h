#pragma once

#include "Engine/Graphics/Shaders/RenderableEffect.h"


namespace bv
{


class GridLinesEffect : public RenderableEffect
{
public:
    GridLinesEffect();
    ~GridLinesEffect();

private:

    PixelShader *       CreatePS        ();
    VertexShader *      CreateVS        ();
    GeometryShader *    CreateGS        ();

};


}	// bv
