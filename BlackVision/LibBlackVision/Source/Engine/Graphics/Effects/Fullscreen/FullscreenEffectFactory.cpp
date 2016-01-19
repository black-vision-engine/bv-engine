#include "FullscreenEffectFactory.h"

#include <cassert>

#include "Engine/Graphics/Effects/Fullscreen/SimpleFullscreenEffect.h"

#include "Engine/Types/Values/ValuesFactory.h"

#include "Engine/Graphics/Effects/Utils/ShaderSourceProvider.h"


namespace bv {

namespace {

// **************************
//
FullscreenEffectTr *    CreateSimpleBlitFSE         ()
{
    FullscreenEffectData fseData;
    auto src = FSEShaderSourceProvider->ReadShader( "blit_no_alpha.frag" );

    fseData.AppendInputTexture( nullptr, "Texture" );
    fseData.SetPixelShaderSource( src );

    return new SimpleFullscreenEffect( fseData );
}

// **************************
//
FullscreenEffectTr *    CreateBlitWithAlphaFSE      ()
{
    FullscreenEffectData fseData;
    auto src = FSEShaderSourceProvider->ReadShader( "blit_alpha.frag" );
    auto val = ValuesFactory::CreateValueFloat( "alpha" );
    val->SetValue( 1.f );

    fseData.AppendInputTexture( nullptr, "Texture" );
    fseData.AppendValue( val );
    fseData.SetPixelShaderSource( src );

    return new SimpleFullscreenEffect( fseData );
}

// **************************
//
FullscreenEffectTr *    CreateBlitWithAlphaMaskFSE  ()
{
    FullscreenEffectData fseData;
    auto src = FSEShaderSourceProvider->ReadShader( "blit_mask_alpha.frag" );
    auto val = ValuesFactory::CreateValueFloat( "alpha" );
    val->SetValue( 1.f );

    fseData.AppendInputTexture( nullptr, "Texture" );
    fseData.AppendInputTexture( nullptr, "Mask" );
    fseData.AppendValue( val );
    fseData.SetPixelShaderSource( src );

    return new SimpleFullscreenEffect( fseData );
}

// **************************
//
FullscreenEffectTr *    CreateInterlaceFSE          ()
{
    FullscreenEffectData fseData;
    auto src = FSEShaderSourceProvider->ReadShader( "interlace.frag" );
    auto val0 = ValuesFactory::CreateValueInt( "startEven" );
    auto val1 = ValuesFactory::CreateValueInt( "height" );
    val0->SetValue( 0 );
    val1->SetValue( 1080 );

    fseData.AppendInputTexture( nullptr, "Tex0" );
    fseData.AppendInputTexture( nullptr, "Tex1" );
    fseData.AppendValue( val0 );
    fseData.AppendValue( val1 );
    fseData.SetPixelShaderSource( src );

    return new SimpleFullscreenEffect( fseData );
}

} // anonymous

// **************************
//
FullscreenEffectTr *    CreateFullscreenEffect( FullscreenEffectTypes fseType )
{
    switch( fseType )
    {
        case FullscreenEffectTypes::FET_SIMPLE_BLIT:
            return CreateSimpleBlitFSE();
        case FullscreenEffectTypes::FET_BLIT_WITH_ALPHA:
            return CreateBlitWithAlphaFSE();
        case FullscreenEffectTypes::FET_BLIT_WITH_ALPHA_MASK:
            return CreateBlitWithAlphaMaskFSE();
        case FullscreenEffectTypes::FET_INTERLACE:
            return CreateInterlaceFSE();
        default:
            assert( false );
    }

    return nullptr;
}

} // bv
