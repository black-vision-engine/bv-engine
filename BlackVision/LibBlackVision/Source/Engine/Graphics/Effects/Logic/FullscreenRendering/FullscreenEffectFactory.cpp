#include "stdafx.h"

#include "FullscreenEffectFactory.h"

#include <cassert>
#include <string>

#include "Engine/Types/Values/ValuesFactory.h"

#include "Engine/Graphics/Effects/Utils/ShaderSourceProvider.h"

#include "Engine/Graphics/Effects/Logic/FullscreenRendering/FullscreenEffectInstance/SimpleFullscreenEffectInstance.h"
#include "Engine/Graphics/Effects/Logic/FullscreenRendering/Impl/FulscreenEffectVisualComponent.h"


namespace bv { 

namespace {

// **************************
//
std::string ReadShaderFromFile( const std::string & fn )
{
    return FSEShaderSourceProvider->ReadShader( fn );
}

// **************************
//
const std::string &		GetPixelShaderFile		                ( FullscreenEffectType nfseType )
{
	return GFetFile[ (unsigned int) nfseType ];
}

// **************************
//
std::string GetPixelShaderSource	                            ( FullscreenEffectType nfseType )
{
	const auto & fn = GetPixelShaderFile( nfseType );

	return ReadShaderFromFile( fn );
}

// **************************
//
FullscreenEffectVisualComponentDesc    CreateDefaultDescriptor ( const std::string & src )
{
    FullscreenEffectVisualComponentDesc d( src );

    d.SetBlendFlag( false );
    d.SetCullFlag( false );
    d.SetDepthTestFlag( false );

    return d;
}

// **************************
// FIXME: nrl - implement it in a separate file (as a part of implementing FSE in a more generic way - so that FSEs don't have to be subclassed for each type - composition to the rescue)
void SetDefaultState( FullscreenEffectVisualComponentDesc * desc, FullscreenEffectType nfseType )
{
    assert( nfseType == FullscreenEffectType::NFET_SIMPLE_BLIT ||
			nfseType == FullscreenEffectType::NFET_PREVIEW_ALPHA_AND_RGB ||
			nfseType == FullscreenEffectType::NFET_BLIT_WITH_ALPHA ||
			nfseType == FullscreenEffectType::NFET_BLIT_WITH_ALPHA_MASK ||
			nfseType == FullscreenEffectType::NFET_BLUR ||
			nfseType == FullscreenEffectType::NFET_LIGHT_SCATTERING ||
			nfseType == FullscreenEffectType::NFET_SHADOW ||
			nfseType == FullscreenEffectType::NFET_GLOW ||
			nfseType == FullscreenEffectType::NFET_SOFT_MASK ||
			nfseType == FullscreenEffectType::NFET_COLOR_CORRECTION ||
			nfseType == FullscreenEffectType::NFET_MIX_CHANNELS );

    desc->SetBlendFlag( false );
    desc->SetCullFlag( false );
    desc->SetDepthTestFlag( false );

    switch( nfseType )
    {
        case FullscreenEffectType::NFET_PREVIEW_ALPHA_AND_RGB:
        case FullscreenEffectType::NFET_SIMPLE_BLIT:
			desc->SetBlendFlag( true );
			desc->AppendInputSamplerEntry( "Texture", TextureWrappingMode::TWM_CLAMP, TextureWrappingMode::TWM_CLAMP, TextureFilteringMode::TFM_LINEAR_MIPMAP_LINEAR );
			desc->AppendIVal( ValuesFactory::CreateValueVec4( "quadTransform", glm::vec4( 1.f, 1.f, 0.f, 0.f ) ) );
			desc->AppendIVal( ValuesFactory::CreateValueVec4( "uvTransform", glm::vec4( 1.f, 1.f, 0.f, 0.f ) ) );
			break;
		case FullscreenEffectType::NFET_BLIT_WITH_ALPHA_MASK:
			desc->SetBlendFlag( true );
			desc->AppendInputSamplerEntry( "Texture" );
			desc->AppendInputSamplerEntry( "Mask" );
            desc->AppendIVal( ValuesFactory::CreateValueInt( "maskChannelIdx", 3 ) );
			desc->AppendIVal( ValuesFactory::CreateValueFloat( "alpha", 1.f ) );
			break;
		case FullscreenEffectType::NFET_BLIT_WITH_ALPHA:
            desc->SetBlendFlag( true );
            desc->AppendInputSamplerEntry( "Texture" );
			desc->AppendIVal( ValuesFactory::CreateValueFloat( "alpha", 1.f ) );
            break;
        case FullscreenEffectType::NFET_COLOR_CORRECTION:
            desc->SetBlendFlag( true );
            desc->AppendInputSamplerEntry( "Texture" );
//            desc->AppendInputSamplerEntry( "Mask" );
            desc->AppendIVal( ValuesFactory::CreateValueFloat( "alpha", 1.f ) );
            desc->AppendIVal( ValuesFactory::CreateValueVec4( "color", glm::vec4( 0.f, 0.f, 0.f, 0.f ) ) );
            break;
        case FullscreenEffectType::NFET_BLUR:
			desc->SetBlendFlag( true );
			desc->AppendInputSamplerEntry( "Tex0", TextureWrappingMode::TWM_CLAMP, TextureWrappingMode::TWM_CLAMP, TextureFilteringMode::TFM_LINEAR );
			desc->AppendIVal( ValuesFactory::CreateValueVec2( "textureSize" ) );
			desc->AppendIVal( ValuesFactory::CreateValueFloat( "blurSize" ) );
			desc->AppendIVal( ValuesFactory::CreateValueBool( "normalize" ) );
			desc->AppendIVal( ValuesFactory::CreateValueBool( "vertical" ) );
			desc->AppendIVal( ValuesFactory::CreateValueInt( "blurKernelType" ) );
			desc->AppendIVal( ValuesFactory::CreateValueVec4( "quadTransform", glm::vec4( 1.f, 1.f, 0.f, 0.f ) ) );
			desc->AppendIVal( ValuesFactory::CreateValueVec4( "uvTransform", glm::vec4( 1.f, 1.f, 0.f, 0.f ) ) );
			break;
		case FullscreenEffectType::NFET_LIGHT_SCATTERING:
			desc->SetBlendFlag( true );
			desc->AppendInputSamplerEntry( "Tex0" );
			desc->AppendIVal( ValuesFactory::CreateValueFloat( "exposure" ) );
			desc->AppendIVal( ValuesFactory::CreateValueFloat( "decay" ) );
			desc->AppendIVal( ValuesFactory::CreateValueFloat( "density" ) );
			desc->AppendIVal( ValuesFactory::CreateValueFloat( "weight" ) );
			desc->AppendIVal( ValuesFactory::CreateValueVec2( "lightPositionOnScreen" ) );
			desc->AppendIVal( ValuesFactory::CreateValueInt( "numSamples" ) );
			break;
		case FullscreenEffectType::NFET_SHADOW:
			desc->SetBlendFlag( true );
			desc->AppendInputSamplerEntry( "BluredTex0" );
			desc->AppendInputSamplerEntry( "Tex0" );
			desc->AppendIVal( ValuesFactory::CreateValueVec2( "textureSize" ) );
			desc->AppendIVal( ValuesFactory::CreateValueVec4( "color" ) );
			desc->AppendIVal( ValuesFactory::CreateValueVec2( "shift" ) );
			desc->AppendIVal( ValuesFactory::CreateValueBool( "inner" ) );
			desc->AppendIVal( ValuesFactory::CreateValueBool( "outer" ) );
			break;
		case FullscreenEffectType::NFET_GLOW: 
			desc->SetBlendFlag( true );
			//desc->SetDepthTestFlag( true );
			desc->AppendInputSamplerEntry( "BluredTex0" );
			desc->AppendInputSamplerEntry( "Tex0" );
			desc->AppendIVal( ValuesFactory::CreateValueVec2( "textureSize" ) );
			desc->AppendIVal( ValuesFactory::CreateValueVec4( "color" ) );
			desc->AppendIVal( ValuesFactory::CreateValueBool( "inner" ) );
			desc->AppendIVal( ValuesFactory::CreateValueBool( "outer" ) );
			desc->AppendIVal( ValuesFactory::CreateValueFloat( "glowStrength" ) );
			break;
		case FullscreenEffectType::NFET_SOFT_MASK:
			desc->SetBlendFlag( true );
			//desc->SetDepthTestFlag( true );
			desc->AppendInputSamplerEntry( "Tex0" );
			desc->AppendIVal( ValuesFactory::CreateValueFloat( "aspectRatio" ) );
			desc->AppendIVal( ValuesFactory::CreateValueFloat( "width" ) );
			desc->AppendIVal( ValuesFactory::CreateValueFloat( "progress" ) );
			desc->AppendIVal( ValuesFactory::CreateValueFloat( "blankWidth" ) );
			desc->AppendIVal( ValuesFactory::CreateValueMat4( "maskTx" ) );
			desc->AppendIVal( ValuesFactory::CreateValueBool( "invert" ) );
			desc->AppendIVal( ValuesFactory::CreateValueBool( "alphaOnly" ) );
			desc->AppendIVal( ValuesFactory::CreateValueBool( "objectOnly" ) );
			desc->AppendIVal( ValuesFactory::CreateValueBool( "mirrorEnabled" ) );
			desc->AppendIVal( ValuesFactory::CreateValueInt( "polyDegree" ) );
			break;
        case FullscreenEffectType::NFET_MIX_CHANNELS:
            desc->AppendInputSamplerEntry( "Texture", TextureWrappingMode::TWM_CLAMP, TextureWrappingMode::TWM_CLAMP, TextureFilteringMode::TFM_LINEAR ); // FIXME: add some room for additional parameters here (e.g. TFM_POINT)
            desc->AppendIVal( ValuesFactory::CreateValueInt( "channelMapping" ) );
            desc->AppendIVal( ValuesFactory::CreateValueVec4( "channelMask" ) );
            break;
        case FullscreenEffectType::NFET_INTERLACE:
        case FullscreenEffectType::NFET_VIDEO_OUTPUT:
        default:
            assert( false );
            break;
    }
}

// **************************
//
FullscreenEffect *   CreateSimpleFSE( FullscreenEffectVisualComponentDesc & desc )
{
    auto vc         = FullscreenEffectVisualComponent::Create( desc );
    auto fseInst    = new SimpleFullscreenEffectInstance( vc );
    auto fse        = new FullscreenEffect( fseInst, desc.GetComponentState() );

    return fse;
}

// **************************
//
FullscreenEffectVisualComponentDesc	CreateDescriptor( FullscreenEffectType nfseType )
{
	auto src		= GetPixelShaderSource( nfseType );
	auto desc       = CreateDefaultDescriptor( src );

	SetDefaultState( &desc, nfseType );

	return desc;
}

} // anonymous


// **************************
// FIXME: add at least basic means of filtering configuration
FullscreenEffect *  CreateFullscreenEffect                  ( FullscreenEffectType nfseType )
{
	auto desc = CreateDescriptor( nfseType );

    return CreateSimpleFSE( desc );
}


} // bv
