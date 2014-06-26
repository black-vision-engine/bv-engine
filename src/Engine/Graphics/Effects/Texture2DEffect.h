#pragma once

#include "Engine/Graphics/Shaders/RenderableEffect.h"


namespace bv {

class Texture2D;

class Texture2DEffect : public RenderableEffect
{
public:

    Texture2DEffect     ( Texture2D * texture );
    ~Texture2DEffect    ();

private:

    PixelShader *   CreatePS    () const;
    VertexShader *  CreateVS    () const;

};

} //bv
