#include "stdafx.h"

#include "FulscreenEffectVisualComponentDesc.h"


namespace bv { 

// **************************
//
FullscreenEffectVisualComponentDesc::FullscreenEffectVisualComponentDesc                            ( const std::string & shaderSrc )
    : m_pixelShaderSrc( shaderSrc )
    , m_componentState( nullptr )
{
    m_componentState = std::make_shared< FullscreenEffectComponentState >();
}

// **************************
//
const std::string &                     FullscreenEffectVisualComponentDesc::GetPixelShaderSrc       () const
{
    return m_pixelShaderSrc;
}

// **************************
//
const Texture2DEntryVec &              FullscreenEffectVisualComponentDesc::GetStaticTextures       () const
{
    return m_componentState->GetAllTextures();
}

// **************************
//
const SamplerEntryVec &                FullscreenEffectVisualComponentDesc::GetInputSamplers          () const
{
    return m_rtInputSamplers;
}

// **************************
//
const IValuePtrVec &                    FullscreenEffectVisualComponentDesc::GetValues               () const
{
    return m_componentState->GetAllValues();
}
	
// **************************
//
const FullscreenEffectRendererState &  FullscreenEffectVisualComponentDesc::GetRendererState        () const
{
    return m_componentState->GetRendererState();
}

// **************************
//
FullscreenEffectComponentStatePtr      FullscreenEffectVisualComponentDesc::GetComponentState       ()
{
    return m_componentState;
}

// **************************
//
void                                    FullscreenEffectVisualComponentDesc::SetPixelShderSrc        ( const std::string & src )
{
    m_pixelShaderSrc = src;
}

// **************************
//
void                                    FullscreenEffectVisualComponentDesc::AppendIVal              ( IValuePtr val )
{
    m_componentState->AppendValue( val );
}

// **************************
//
void                                    FullscreenEffectVisualComponentDesc::AppendTextureEntry      ( Texture2DPtr tex, const std::string & samplerName )
{
    AppendTextureEntry( tex, samplerName, TextureWrappingMode::TWM_CLAMP, TextureWrappingMode::TWM_CLAMP, TextureFilteringMode::TFM_POINT );
}

// **************************
//
void                                    FullscreenEffectVisualComponentDesc::AppendTextureEntry      ( Texture2DPtr tex, const std::string & samplerName, TextureWrappingMode wrapX, TextureWrappingMode wrapY, TextureFilteringMode filteringMode )
{
    m_componentState->AppendTexture( tex, samplerName, wrapX, wrapY, filteringMode );
}

// **************************
//
void                                    FullscreenEffectVisualComponentDesc::AppendInputSamplerEntry ( const std::string & samplerName )
{
    AppendInputSamplerEntry( samplerName, TextureWrappingMode::TWM_CLAMP, TextureWrappingMode::TWM_CLAMP, TextureFilteringMode::TFM_POINT );
}

// **************************
//
void                                    FullscreenEffectVisualComponentDesc::AppendInputSamplerEntry ( const std::string & samplerName, TextureWrappingMode wrapX, TextureWrappingMode wrapY, TextureFilteringMode filteringMode )
{
    m_rtInputSamplers.push_back( SamplerEntry( samplerName, wrapX, wrapY, filteringMode ) );
}

// **************************
//
void                                    FullscreenEffectVisualComponentDesc::SetBlendFlag            ( bool flag )
{
    m_componentState->AccessRendererState().SetBlendFlag( flag );
}

// **************************
//
void                                    FullscreenEffectVisualComponentDesc::SetDepthTestFlag        ( bool flag )
{
    m_componentState->AccessRendererState().SetDepthTestFlag( flag );
}

// **************************
//
void                                    FullscreenEffectVisualComponentDesc::SetCullFlag             ( bool flag )
{
    m_componentState->AccessRendererState().SetCullFlag( flag );
}


} // bv
