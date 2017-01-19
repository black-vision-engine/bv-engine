#include "stdafx.h"

#include "NFullscreenEffectFactory.h"

#include <cassert>
#include <string>

#include "Engine/Types/Values/ValuesFactory.h"

#include "Engine/Graphics/Effects/Utils/ShaderSourceProvider.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/FullscreenEffectInstance/NSimpleFullscreenEffectInstance.h"
#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/Impl/NFulscreenEffectVisualComponent.h"


namespace bv { namespace nrl {

namespace {

// **************************
//
std::string ReadShaderFromFile( const std::string & fn )
{
    return FSEShaderSourceProvider->ReadShader( fn );
}

// **************************
//
const std::string &		GetPixelShaderFile		                ( NFullscreenEffectType nfseType )
{
	return GFetFile[ (unsigned int) nfseType ];
}

// **************************
//
std::string GetPixelShaderSource	                            ( NFullscreenEffectType nfseType )
{
	const auto & fn = GetPixelShaderFile( nfseType );

	return ReadShaderFromFile( fn );
}

// **************************
//
NFullscreenEffectVisualComponentDesc    CreateDefaultDescriptor ( const std::string & src )
{
    NFullscreenEffectVisualComponentDesc d( src );

    d.SetBlendFlag( false );
    d.SetCullFlag( false );
    d.SetDepthTestFlag( false );

    return d;
}

// **************************
// FIXME: nrl - implement it in a separate file (as a part of implementing FSE in a more generic way - so that FSEs don't have to be subclassed for each type - composition to the rescue)
void SetDefaultState( NFullscreenEffectVisualComponentDesc * desc, NFullscreenEffectType nfseType )
{
    assert( nfseType == NFullscreenEffectType::NFET_SIMPLE_BLIT ||
			nfseType == NFullscreenEffectType::NFET_PREVIEW_ALPHA_AND_RGB ||
			nfseType == NFullscreenEffectType::NFET_BLIT_WITH_ALPHA ||
			nfseType == NFullscreenEffectType::NFET_BLIT_WITH_ALPHA_MASK ||
			nfseType == NFullscreenEffectType::NFET_BLUR ||
			nfseType == NFullscreenEffectType::NFET_LIGHT_SCATTERING ||
			nfseType == NFullscreenEffectType::NFET_SHADOW ||
			nfseType == NFullscreenEffectType::NFET_GLOW);

    desc->SetBlendFlag( false );
    desc->SetCullFlag( false );
    desc->SetDepthTestFlag( false );

    switch( nfseType )
    {
        case NFullscreenEffectType::NFET_PREVIEW_ALPHA_AND_RGB:
        case NFullscreenEffectType::NFET_SIMPLE_BLIT:
			desc->AppendInputSamplerEntry( "Texture" );
			break;
		case NFullscreenEffectType::NFET_BLIT_WITH_ALPHA_MASK:
            desc->AppendInputSamplerEntry( "Mask" );
            desc->AppendIVal( ValuesFactory::CreateValueInt( "maskChannelIdx", 3 ) );
		case NFullscreenEffectType::NFET_BLIT_WITH_ALPHA:
            desc->SetBlendFlag( true );
            desc->AppendInputSamplerEntry( "Texture" );
            desc->AppendIVal( ValuesFactory::CreateValueFloat( "alpha", 1.f ) );
            break;
		case NFullscreenEffectType::NFET_BLUR:
			desc->AppendInputSamplerEntry( "Tex0" );
			desc->AppendIVal( ValuesFactory::CreateValueVec2( "textureSize" ) );
			desc->AppendIVal( ValuesFactory::CreateValueFloat( "blurSize" ) );
			desc->AppendIVal( ValuesFactory::CreateValueBool( "normalize" ) );
			desc->AppendIVal( ValuesFactory::CreateValueBool( "vertical" ) );
			desc->AppendIVal( ValuesFactory::CreateValueInt( "blurKernelType" ) );
			break;
		case NFullscreenEffectType::NFET_LIGHT_SCATTERING:
			desc->AppendInputSamplerEntry( "Tex0" );
			desc->AppendIVal( ValuesFactory::CreateValueFloat( "exposure" ) );
			desc->AppendIVal( ValuesFactory::CreateValueFloat( "decay" ) );
			desc->AppendIVal( ValuesFactory::CreateValueFloat( "density" ) );
			desc->AppendIVal( ValuesFactory::CreateValueFloat( "weight" ) );
			desc->AppendIVal( ValuesFactory::CreateValueVec2( "lightPositionOnScreen" ) );
			desc->AppendIVal( ValuesFactory::CreateValueInt( "numSamples" ) );
			break;
		case NFullscreenEffectType::NFET_SHADOW:
			desc->AppendInputSamplerEntry( "BluredTex0" );
			desc->AppendInputSamplerEntry( "Tex0" );
			desc->AppendIVal( ValuesFactory::CreateValueVec2( "textureSize" ) );
			desc->AppendIVal( ValuesFactory::CreateValueVec4( "color" ) );
			desc->AppendIVal( ValuesFactory::CreateValueVec2( "shift" ) );
			desc->AppendIVal( ValuesFactory::CreateValueBool( "inner" ) );
			desc->AppendIVal( ValuesFactory::CreateValueBool( "outer" ) );
			break;
		case NFullscreenEffectType::NFET_GLOW:
			desc->SetBlendFlag( true );
			desc->SetDepthTestFlag( true );
			desc->AppendInputSamplerEntry( "BluredTex0" );
			desc->AppendInputSamplerEntry( "Tex0" );
			desc->AppendIVal( ValuesFactory::CreateValueVec2( "textureSize" ) );
			desc->AppendIVal( ValuesFactory::CreateValueVec4( "color" ) );
			desc->AppendIVal( ValuesFactory::CreateValueBool( "inner" ) );
			desc->AppendIVal( ValuesFactory::CreateValueBool( "outer" ) );
			desc->AppendIVal( ValuesFactory::CreateValueFloat( "glowStrength" ) );
			break;
        case NFullscreenEffectType::NFET_INTERLACE:
        case NFullscreenEffectType::NFET_MIX_CHANNELS:
        case NFullscreenEffectType::NFET_VIDEO_OUTPUT:
        default:
            assert( false );
            break;
    }
}

// **************************
//
NFullscreenEffect *   CreateSimpleFSE( NFullscreenEffectVisualComponentDesc & desc )
{
    auto vc         = NFullscreenEffectVisualComponent::Create( desc );
    auto fseInst    = new NSimpleFullscreenEffectInstance( vc );
    auto fse        = new NFullscreenEffect( fseInst, desc.GetComponentState() );

    return fse;
}

// **************************
//
NFullscreenEffectVisualComponentDesc	CreateDescriptor( NFullscreenEffectType nfseType )
{
	auto src		= GetPixelShaderSource( nfseType );
	auto desc       = CreateDefaultDescriptor( src );

	SetDefaultState( &desc, nfseType );

	return desc;
}

} // anonymous


// **************************
//
NFullscreenEffect *  CreateFullscreenEffect                  ( NFullscreenEffectType nfseType )
{
	auto desc = CreateDescriptor( nfseType );

    return CreateSimpleFSE( desc );
}

} // nrl
} // bv
