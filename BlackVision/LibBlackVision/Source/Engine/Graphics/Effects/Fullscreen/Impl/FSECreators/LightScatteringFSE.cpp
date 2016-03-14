#include "stdafx.h"

#include "LightScatteringFSE.h"

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
FullscreenEffect *       LightScatteringFSECreator::CreateLightScatteringFSE    ( const std::vector< IValuePtr > & values )
{
    FullscreenEffectData fseData;

    fseData.AppendInputTexture( nullptr, "Tex0" );

    auto exposureVal = ValuesFactory::CreateValueFloat( "exposure" );
    auto decayVal = ValuesFactory::CreateValueFloat( "decay" );
    auto densityVal = ValuesFactory::CreateValueFloat( "density" );
    auto weightVal = ValuesFactory::CreateValueFloat( "weight" );
    auto numSamplesVal = ValuesFactory::CreateValueFloat( "numSamples" );
    auto positionVal = ValuesFactory::CreateValueVec2( "position" );
    auto lightPositionOnScreenVal = ValuesFactory::CreateValueVec2( "lightPositionOnScreen" );

    fseData.AppendValues( values );
    fseData.AppendValue( exposureVal );
    fseData.AppendValue( decayVal );
    fseData.AppendValue( densityVal );
    fseData.AppendValue( weightVal );
    fseData.AppendValue( numSamplesVal );
    fseData.AppendValue( positionVal );
    fseData.AppendValue( lightPositionOnScreenVal );

    auto src = FSEShaderSourceProvider->ReadShader( "light_scattering.frag" );
    fseData.SetPixelShaderSource( src );

    fseData.SetBlendEnabled( false );
    fseData.SetCullEnabled( false );
    fseData.SetDepthTestEnabled( false );

    return new SimpleFullscreenEffect( fseData );
}

} // bv