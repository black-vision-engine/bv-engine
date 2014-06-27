#include "Texture2DEffect.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParameters.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"


namespace bv {

// ****************************
//
Texture2DEffect::Texture2DEffect     ( Texture2D * texture, bool hasAlpha )
{
    auto ps = CreatePS( texture, hasAlpha );
    auto vs = CreateVS();

    RenderablePass * pass = new RenderablePass( ps, vs, nullptr );

    AddPass( pass );
}

// ****************************
//
Texture2DEffect::~Texture2DEffect    ()
{
    auto pass = GetPass( 0 );

    delete pass->GetPixelShader();
    delete pass->GetVertexShader();
}

// ****************************
//
PixelShader *   Texture2DEffect::CreatePS    ( Texture2D * texture, bool hasAlpha ) const
{
    auto sp = new ShaderParameters();

    if( hasAlpha )
    {
        GenericShaderParam * param = ShaderParamFactory::CreateGenericParameter( "alpha", ParamType::PT_FLOAT1 );
        assert( param != nullptr );

        sp->AddParameter( param );
    }

    return nullptr;
}

// ****************************
//
VertexShader *  Texture2DEffect::CreateVS    () const
{
    return nullptr;
}

} //bv
