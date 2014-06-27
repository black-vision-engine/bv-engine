#pragma once

#include "Engine/Graphics/Shaders/RenderableEffect.h"


namespace bv {

class Texture2D;

class Texture2DEffect : public RenderableEffect
{
public:

    Texture2DEffect     ( Texture2D * texture, bool hasAlpha = false );
    ~Texture2DEffect    ();

private:

    PixelShader *   CreatePS    ( Texture2D * texture, bool hasAlpha ) const;
    VertexShader *  CreateVS    () const;

};

} //bv
