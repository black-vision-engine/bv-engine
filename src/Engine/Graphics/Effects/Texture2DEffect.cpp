#include "Texture2DEffect.h"

namespace bv {

// ****************************
//
Texture2DEffect::Texture2DEffect     ( Texture2D * texture )
{
    auto ps = nullptr;
    auto vs = nullptr;
    auto gs = nullptr;

    RenderablePass * pass = new RenderablePass( ps, vs, gs );

    AddPass( pass );
}

// ****************************
//
Texture2DEffect::~Texture2DEffect    ()
{
    auto pass = GetPass( 0 );

    delete pass->GetPixelShader();
    delete pass->GetVertexShader();
    delete pass->GetGeometryShader();
}

// ****************************
//
PixelShader *   Texture2DEffect::CreatePS    () const
{
    return nullptr;
}

// ****************************
//
VertexShader *  Texture2DEffect::CreateVS    () const
{
    return nullptr;
}

} //bv
