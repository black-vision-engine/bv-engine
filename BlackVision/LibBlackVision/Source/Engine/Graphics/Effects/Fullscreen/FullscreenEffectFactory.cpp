#include "stdafx.h"

#include "FullscreenEffectFactory.h"

#include <cassert>

#include "Engine/Graphics/Effects/Fullscreen/Impl/SimpleFullscreenEffect.h"
#include "Engine/Graphics/Effects/Fullscreen/Impl/CompositeFullscreenEffect.h"
#include "Engine/Graphics/Effects/Fullscreen/Impl/Graph/FullscreenEffectGraph.h"

#include "Engine/Types/Values/ValuesFactory.h"

#include "Engine/Graphics/Effects/Utils/ShaderSourceProvider.h"

#include "Engine/Graphics/Effects/Fullscreen/Impl/FSECreators/BlurFSE.h"
#include "Engine/Graphics/Effects/Fullscreen/Impl/FSECreators/ShadowFSE.h"
#include "Engine/Graphics/Effects/Fullscreen/Impl/FSECreators/ImageMaskFSE.h"
#include "Engine/Graphics/Effects/Fullscreen/Impl/FSECreators/LightScatteringMaskFSE.h"

namespace bv {

namespace {
    
// **************************
//
FullscreenEffect *  CreateSimpleBlitFSE         ( const std::vector< IValuePtr > & values )
{
    FullscreenEffectData fseData;
    auto src = FSEShaderSourceProvider->ReadShader( "blit_no_alpha.frag" );

    fseData.AppendValues( values );

    fseData.AppendInputTexture( nullptr, "Texture" );
    fseData.SetPixelShaderSource( src );
    fseData.SetBlendEnabled( false );
    fseData.SetCullEnabled( false );
    fseData.SetDepthTestEnabled( false );

    return new SimpleFullscreenEffect( fseData );
}

// **************************
//
FullscreenEffect *  CreateBlitWithAlphaFSE      ( const std::vector< IValuePtr > & values )
{
    FullscreenEffectData fseData;
    auto src = FSEShaderSourceProvider->ReadShader( "blit_alpha.frag" );

    assert( values.size() == 1 );
    assert( values[ 0 ]->GetName() == "alpha" );

    fseData.AppendValues( values );

    fseData.AppendInputTexture( nullptr, "Texture" );
    fseData.SetPixelShaderSource( src );
    fseData.SetBlendEnabled( true );
    fseData.SetCullEnabled( false );
    fseData.SetDepthTestEnabled( false );

    return new SimpleFullscreenEffect( fseData );
}

// **************************
//
FullscreenEffect *  CreateBlitWithAlphaMaskFSE  ( const std::vector< IValuePtr > & values )
{
    FullscreenEffectData fseData;
    auto src = FSEShaderSourceProvider->ReadShader( "blit_mask_alpha.frag" );

    assert( values.size() == 3 );
    assert( values[ 0 ]->GetName() == "alpha" );
    assert( values[ 1 ]->GetName() == "maskIdx" );
    assert( values[ 2 ]->GetName() == "fgIdx" );

    fseData.AppendValues( values );

    fseData.AppendInputTexture( nullptr, "Texture" );
    fseData.AppendInputTexture( nullptr, "Mask" );
    fseData.SetPixelShaderSource( src );
    fseData.SetBlendEnabled( true );
    fseData.SetCullEnabled( false );
    fseData.SetDepthTestEnabled( false );

    return new SimpleFullscreenEffect( fseData );
}

// **************************
//
FullscreenEffect *  CreateInterlaceFSE          ( const std::vector< IValuePtr > & values )
{
    FullscreenEffectData fseData;
    auto src = FSEShaderSourceProvider->ReadShader( "interlace.frag" );

    assert( values.size() == 0 );   { values; }

    auto val0 = ValuesFactory::CreateValueInt( "startEven", 0 );
    auto val1 = ValuesFactory::CreateValueInt( "height", 1080 );

    fseData.AppendInputTexture( nullptr, "Tex0" );
    fseData.AppendInputTexture( nullptr, "Tex1" );

    fseData.AppendValue( val0 );
    fseData.AppendValue( val1 );

    fseData.SetPixelShaderSource( src );

    fseData.SetBlendEnabled( false );
    fseData.SetCullEnabled( false );
    fseData.SetDepthTestEnabled( false );

    return new SimpleFullscreenEffect( fseData );
}

// **************************
//
FullscreenEffect *  CreateMixChannelsFSE        ( const std::vector< IValuePtr > & values )
{
    FullscreenEffectData fseData;
    auto src = FSEShaderSourceProvider->ReadShader( "mixchannels.frag" );

    assert( values.size() == 0 );   { values; }

    // ( ( aIdx & 0x3 ) << 6 ) | ( ( bIdx & 0x3 ) << 4 ) | ( ( gIdx & 0x3 ) << 2 ) | ( ( rIdx & 0x3 ) << 0 );
    auto val0 = ValuesFactory::CreateValueInt( "channelMask", ( ( 3 & 0x3 ) << 6 ) | ( ( 2 & 0x3 ) << 4 ) | ( ( 1 & 0x3 ) << 2 ) | ( ( 0 & 0x3 ) << 0 ) );

    fseData.AppendInputTexture( nullptr, "Tex0" );

    fseData.AppendValue( val0 );

    fseData.SetPixelShaderSource( src );

    fseData.SetBlendEnabled( false );
    fseData.SetCullEnabled( false );
    fseData.SetDepthTestEnabled( false );

    return new SimpleFullscreenEffect( fseData );
}

// **************************
//
FullscreenEffect *  CreateVideoOutputFSE        ( const std::vector< IValuePtr > & values )
{
    FullscreenEffectData fseData;
    auto src = FSEShaderSourceProvider->ReadShader( "videooutput.frag" );

    assert( values.size() == 0 );   { values; }

    // ( ( aIdx & 0x3 ) << 6 ) | ( ( bIdx & 0x3 ) << 4 ) | ( ( gIdx & 0x3 ) << 2 ) | ( ( rIdx & 0x3 ) << 0 );
    int channelMask         = ( 3 << 6 ) | ( 2 << 4 ) | ( 1 << 2 ) | ( 0 << 0 );

    auto useInterlaceVal    = ValuesFactory::CreateValueInt     ( "useInterlace", 1 );
    auto startEvenVal       = ValuesFactory::CreateValueInt     ( "startEven", 0 );
    auto heightVal          = ValuesFactory::CreateValueFloat   ( "height", 1080 );
    auto channelMaskVal     = ValuesFactory::CreateValueInt     ( "channelMask", channelMask ); 
    auto alphaVal           = ValuesFactory::CreateValueFloat   ( "alpha", 1.f );
    auto overwriteAlphaVal  = ValuesFactory::CreateValueInt     ( "overwriteAlpha", 0 );

    fseData.AppendInputTexture( nullptr, "Tex0" );
    fseData.AppendInputTexture( nullptr, "Tex1" );

    fseData.AppendValue( useInterlaceVal );
    fseData.AppendValue( startEvenVal );
    fseData.AppendValue( heightVal );
    fseData.AppendValue( channelMaskVal );
    fseData.AppendValue( alphaVal );
    fseData.AppendValue( overwriteAlphaVal );

    fseData.SetPixelShaderSource( src );

    fseData.SetBlendEnabled( false );
    fseData.SetCullEnabled( false );
    fseData.SetDepthTestEnabled( false );

    return new SimpleFullscreenEffect( fseData );
}

} // anonymous

// **************************
//
FullscreenEffect *  CreateFullscreenEffect( FullscreenEffectType fseType, const std::vector< IValuePtr > & values )
{
    switch( fseType )
    {
        case FullscreenEffectType::FET_SIMPLE_BLIT:
        {
            return CreateSimpleBlitFSE( values );
        }
        case FullscreenEffectType::FET_BLIT_WITH_ALPHA:
        {
            return CreateBlitWithAlphaFSE( values );
        }
        case FullscreenEffectType::FET_BLIT_WITH_ALPHA_MASK:
        {
            return CreateBlitWithAlphaMaskFSE( values );
        }
        case FullscreenEffectType::FET_INTERLACE:
        {
            return CreateInterlaceFSE( values );
        }
        case FullscreenEffectType::FET_MIX_CHANNELS:
        {
            return CreateMixChannelsFSE( values );
        }
        case FullscreenEffectType::FET_VIDEO_OUTPUT:
        {
            return CreateVideoOutputFSE( values );
        }
        case FullscreenEffectType::FET_BLUR:
        {
            return BlurFSECreator::CreateCompositeFSE( values );
            //return BlurFSECreator::CreateCompositeOneFSE( values );
            //return BlurFSECreator::CreateSimpleFSE( values );
        }        
        case FullscreenEffectType::FET_SHADOW:
        {
            return ShadowFSECreator::CreateShadowFSE( values );
        }
        case FullscreenEffectType::FET_IMAGE_MASK:
        {
            return ImageMaskFSECreator::CreateImageMaskFSE( values );
        }
        case FullscreenEffectType::FET_LIGHT_SCATTERING:
        {
            return LightScatteringMaskFSECreator::CreateLightScatteringMaskFSE( values );
        }
        default:
            assert( false );
    }

    return nullptr;
}

// **************************
//
FullscreenEffect *  CreateFullscreenEffect( FullscreenEffectType fseType )
{
    return CreateFullscreenEffect( fseType, std::vector< IValuePtr >() );
}

// **************************
//
FullscreenEffectInstance *  CreateFullscreenEffectInstance  ( FullscreenEffectType fseType, const std::vector< IValuePtr > & values )
{
    auto effect = std::shared_ptr< FullscreenEffect >( CreateFullscreenEffect( fseType, values ) );

    return new FullscreenEffectInstance( effect );
}

// **************************
//
FullscreenEffectInstance *  CreateFullscreenEffectInstance  ( FullscreenEffectType fseType )
{
    auto effect = std::shared_ptr< FullscreenEffect >( CreateFullscreenEffect( fseType ) );

    return new FullscreenEffectInstance( effect );
}

} // bv
