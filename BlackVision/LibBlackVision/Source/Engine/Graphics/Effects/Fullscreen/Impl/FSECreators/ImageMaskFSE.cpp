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
FullscreenEffect *       ImageMaskFSECreator::CreateImageMaskFSE    ( const std::vector< IValuePtr > & values )
{
    FullscreenEffectData fseData;

    fseData.AppendInputTexture( nullptr, "Tex0" );
    fseData.AppendInputTexture( nullptr, "Mask0", true );

    auto blendVal = ValuesFactory::CreateValueFloat( "blend" );
    auto positionVal = ValuesFactory::CreateValueVec2( "position" );
    auto scaleVal = ValuesFactory::CreateValueVec2( "scale" );
    auto invertVal = ValuesFactory::CreateValueInt( "invert" );
    //auto fitVal = ValuesFactory::CreateValueInt( "fit" );
    auto wrapVal = ValuesFactory::CreateValueInt( "wrap" );
    //auto maskAspectVal = ValuesFactory::CreateValueInt( "maskAspect" );
    auto alphaOnlyVal = ValuesFactory::CreateValueInt( "alphaOnly" );
    auto softMaskVal = ValuesFactory::CreateValueInt( "softMask" );

    fseData.AppendValues( values );
    fseData.AppendValue( blendVal );
    fseData.AppendValue( positionVal );
    fseData.AppendValue( scaleVal );
    fseData.AppendValue( invertVal );
    //fseData.AppendValue( fitVal );
    fseData.AppendValue( wrapVal );
    //fseData.AppendValue( maskAspectVal );
    fseData.AppendValue( alphaOnlyVal );
    fseData.AppendValue( softMaskVal );

    auto src = FSEShaderSourceProvider->ReadShader( "image_mask.frag" );
    fseData.SetPixelShaderSource( src );

    fseData.SetBlendEnabled( true );
    fseData.SetCullEnabled( false );
    fseData.SetDepthTestEnabled( false );

    return new SimpleFullscreenEffect( fseData );
}

} // bv