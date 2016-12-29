#include "stdafx.h"

#include "NFulscreenEffectVisualComponentDesc.h"


namespace bv { namespace nrl {

// **************************
//
NFullscreenEffectVisualComponentDesc::NFullscreenEffectVisualComponentDesc                            ( const std::string & shaderSrc )
    : m_pixelShaderSrc( shaderSrc )
    , m_componentState( nullptr )
{
    m_componentState = std::make_shared< NFullscreenEffectComponentState >();
}

// **************************
//
const std::string &                     NFullscreenEffectVisualComponentDesc::GetPixelShaderSrc       () const
{
    return m_pixelShaderSrc;
}

// **************************
//
const NTexture2DEntryVec &              NFullscreenEffectVisualComponentDesc::GetStaticTextures       () const
{
    return m_componentState->GetAllTextures();
}

// **************************
//
const std::vector< std::string > &      NFullscreenEffectVisualComponentDesc::GetInputSamplerNames    () const
{
    return m_rtInputSamplerNames;
}

// **************************
//
const IValuePtrVec &                    NFullscreenEffectVisualComponentDesc::GetValues               () const
{
    return m_componentState->GetAllValues();
}
	
// **************************
//
const NFullscreenEffectRendererState &  NFullscreenEffectVisualComponentDesc::GetRendererState        () const
{
    return m_componentState->GetRendererState();
}

// **************************
//
NFullscreenEffectComponentStatePtr      NFullscreenEffectVisualComponentDesc::GetComponentState       ()
{
    return m_componentState;
}

// **************************
//
void                                    NFullscreenEffectVisualComponentDesc::SetPixelShderSrc        ( const std::string & src )
{
    m_pixelShaderSrc = src;
}

// **************************
//
void                                    NFullscreenEffectVisualComponentDesc::AppendIVal              ( IValuePtr val )
{
    m_componentState->AppendValue( val );
}

// **************************
//
void                                    NFullscreenEffectVisualComponentDesc::AppendTextureEntry      ( Texture2DPtr tex, const std::string & samplerName )
{
    AppendTextureEntry( tex, samplerName, TextureWrappingMode::TWM_CLAMP, TextureWrappingMode::TWM_CLAMP, TextureFilteringMode::TFM_POINT );
}

// **************************
//
void                                    NFullscreenEffectVisualComponentDesc::AppendTextureEntry      ( Texture2DPtr tex, const std::string & samplerName, TextureWrappingMode wrapX, TextureWrappingMode wrapY, TextureFilteringMode filteringMode )
{
    m_componentState->AppendTexture( tex, samplerName, wrapX, wrapY, filteringMode );
}

// **************************
//
void                                    NFullscreenEffectVisualComponentDesc::AppendInputSamplerName  ( const std::string & samplerName )
{
    m_rtInputSamplerNames.push_back( samplerName );
}

// **************************
//
void                                    NFullscreenEffectVisualComponentDesc::SetBlendFlag            ( bool flag )
{
    m_componentState->AccessRendererState().SetBlendFlag( flag );
}

// **************************
//
void                                    NFullscreenEffectVisualComponentDesc::SetDepthTestFlag        ( bool flag )
{
    m_componentState->AccessRendererState().SetDepthTestFlag( flag );
}

// **************************
//
void                                    NFullscreenEffectVisualComponentDesc::SetCullFlag             ( bool flag )
{
    m_componentState->AccessRendererState().SetCullFlag( flag );
}

} // nrl
} // bv
