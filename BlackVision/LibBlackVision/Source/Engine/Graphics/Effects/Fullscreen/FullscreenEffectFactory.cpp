#include "FullscreenEffectFactory.h"

#include <cassert>

#include "Engine/Graphics/Effects/Fullscreen/Impl/SimpleFullscreenEffect.h"

#include "Engine/Types/Values/ValuesFactory.h"

#include "Engine/Graphics/Effects/Utils/ShaderSourceProvider.h"


namespace bv {

namespace {

// **************************
//
 void AppendValues( FullscreenEffectData * fseData, const std::vector< IValuePtr > & values )
 {
    for( auto val : values )
    {
        fseData->AppendValue( val );
    }
 }
    
// **************************
//
FullscreenEffect *  CreateSimpleBlitFSE         ( const std::vector< IValuePtr > & values )
{
    FullscreenEffectData fseData;
    auto src = FSEShaderSourceProvider->ReadShader( "blit_no_alpha.frag" );

    AppendValues( &fseData, values );

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

    AppendValues( &fseData, values );

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

    AppendValues( &fseData, values );

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

    assert( values.size() == 0 );

    auto val0 = ValuesFactory::CreateValueInt( "startEven" );
    auto val1 = ValuesFactory::CreateValueInt( "height" );
    val0->SetValue( 0 );
    val1->SetValue( 1080 );

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

    assert( values.size() == 0 );

    auto val0 = ValuesFactory::CreateValueInt( "channelMask" );

    val0->SetValue( ( ( 3 & 0x3 ) << 6 ) | ( ( 2 & 0x3 ) << 4 ) | ( ( 1 & 0x3 ) << 2 ) | ( ( 0 & 0x3 ) << 0 ) );

    fseData.AppendInputTexture( nullptr, "Tex0" );
    fseData.AppendValue( val0 );
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
