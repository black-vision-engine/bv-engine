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

    auto fse = new SimpleFullscreenEffect( fseData );

    return fse;
}

// **************************
//
FullscreenEffectTr *    CreateBlitWithAlphaFSE      ()
{
    return nullptr;
}

// **************************
//
FullscreenEffectTr *    CreateBlitWithAlphaMaskFSE  ()
{
    return nullptr;
}

// **************************
//
FullscreenEffectTr *    CreateInterlaceFSE          ()
{
    return nullptr;
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
