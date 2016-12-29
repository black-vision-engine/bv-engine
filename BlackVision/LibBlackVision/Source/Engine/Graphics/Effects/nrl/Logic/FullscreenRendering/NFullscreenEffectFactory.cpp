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
//
void SetDefaultState( NFullscreenEffectVisualComponentDesc * desc, NFullscreenEffectType nfseType )
{
    assert( nfseType == NFullscreenEffectType::NFET_SIMPLE_BLIT || nfseType == NFullscreenEffectType::NFET_PREVIEW_ALPHA_AND_RGB || nfseType == NFullscreenEffectType::NFET_BLIT_WITH_ALPHA || nfseType == NFullscreenEffectType::NFET_BLIT_WITH_ALPHA_MASK );

    desc->SetBlendFlag( false );
    desc->SetCullFlag( false );
    desc->SetDepthTestFlag( false );

    switch( nfseType )
    {
        case NFullscreenEffectType::NFET_PREVIEW_ALPHA_AND_RGB:
        case NFullscreenEffectType::NFET_SIMPLE_BLIT:
			desc->AppendInputSamplerName( "Texture" );
			break;
		case NFullscreenEffectType::NFET_BLIT_WITH_ALPHA_MASK:
            desc->AppendInputSamplerName( "Mask" );
		case NFullscreenEffectType::NFET_BLIT_WITH_ALPHA:
            desc->SetBlendFlag( true );
            desc->AppendInputSamplerName( "Texture" );
            desc->AppendIVal( ValuesFactory::CreateValueFloat( "alpha", 1.f ) );
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
