#include "stdafx.h"

#include "ImageMaskFSE.h"

#include "Engine/Graphics/Effects/Fullscreen/Impl/SimpleFullscreenEffect.h"
#include "Engine/Graphics/Effects/Fullscreen/Impl/CompositeFullscreenEffect.h"
#include "Engine/Graphics/Effects/Fullscreen/Impl/Graph/FullscreenEffectGraph.h"

#include "Engine/Types/Values/ValuesFactory.h"

#include "Engine/Graphics/Effects/Utils/ShaderSourceProvider.h"

#include "ImageMaskFSE.h"

namespace bv
{

// **************************
//
FullscreenEffect *       ImageMaskFSECreator::CreateImageMaskFSE    ( const std::vector< IValuePtr > & )
{
    FullscreenEffectData fseData;

    fseData.AppendInputTexture( nullptr, "Tex0" );

    auto src = FSEShaderSourceProvider->ReadShader( "image_mask.frag" );
    fseData.SetPixelShaderSource( src );

    fseData.SetBlendEnabled( false );
    fseData.SetCullEnabled( false );
    fseData.SetDepthTestEnabled( false );

    return new SimpleFullscreenEffect( fseData );
}

} // bv