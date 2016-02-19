#include "stdafx.h"

#include "DefaultTexturesData.h"

#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureDescriptor.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultAnimationDescriptor.h"

#include "Engine/Models/AssetTracker.h"


namespace bv { namespace model {

// ******************************
//
DefaultTexturesData::DefaultTexturesData ()
{
}

// ******************************
//
DefaultTexturesData::~DefaultTexturesData()
{
}

// ******************************
//
const std::vector< ITextureDescriptorPtr > &     DefaultTexturesData::GetTextures        () const
{
    return m_textureDescriptors;
}

// ******************************
//
const std::vector< IAnimationDescriptorPtr > &   DefaultTexturesData::GetAnimations      () const
{
    return m_animationDescriptors;
}

// ******************************
//
const std::vector< IFontDescriptorPtr > &       DefaultTexturesData::GetFonts        () const
{
    return m_fontDescriptors;
}

// ******************************
//
ITextureDescriptorPtr                            DefaultTexturesData::GetTexture          ( unsigned int idx )
{
    if( idx < m_textureDescriptors.size() )
	{
		return m_textureDescriptors[ idx ];
	}
	return nullptr;
}

// ******************************
//
IAnimationDescriptorPtr                          DefaultTexturesData::GetAnimation        ( unsigned int idx )
{
    assert( idx < m_animationDescriptors.size() );

    return m_animationDescriptors[ idx ];
}

// ******************************
//
ITextureDescriptorPtr                            DefaultTexturesData::GetTexture          ( const std::string & name )
{
	for( auto tx : m_textureDescriptors )
	{
		if( tx->GetName() == name )
		{
			return tx;
		}
	}
	return nullptr;
}

// ******************************
//
void                                            DefaultTexturesData::SetTexture         ( unsigned int idx, ITextureDescriptorPtr textureDesc )
{
    if( idx >= m_textureDescriptors.size() )
	{
		m_textureDescriptors.resize( idx + 1 );
	}

    if( m_textureDescriptors[ idx ] )
    {
        AssetTracker::Instance().UnregisterAsset( m_textureDescriptors[ idx ] );
    }

	m_textureDescriptors[ idx ] = textureDesc;
    AssetTracker::Instance().RegisterAsset( textureDesc );
}

// ******************************
//
void                                            DefaultTexturesData::AddTexture         ( ITextureDescriptorPtr textureDesc )
{
    m_textureDescriptors.push_back( textureDesc );
    AssetTracker::Instance().RegisterAsset( textureDesc );
}

// ******************************
//
void                                            DefaultTexturesData::SetAnimation       ( unsigned int idx, IAnimationDescriptorPtr animationDesc )
{
	if( idx >= m_animationDescriptors.size() )
	{
		m_animationDescriptors.resize( idx + 1 );
	} 
	m_animationDescriptors[ idx ] = animationDesc;
}

// ******************************
//
void                                            DefaultTexturesData::AddAnimation       ( IAnimationDescriptorPtr animationDesc )
{
    m_animationDescriptors.push_back( animationDesc );
}

// ******************************
//
bool                                            DefaultTexturesData::SetAnimationFrame   ( unsigned int idx, unsigned int frameNum )
{
    //assert( idx < m_animationDescriptors.size() ); // FUNKED for serialization
    if( idx < m_animationDescriptors.size() )
	{
		auto desc = std::static_pointer_cast< DefaultAnimationDescriptor >( m_animationDescriptors[ idx ] );
		desc->SetCurrentFrame( frameNum );
		return true;
	}
    return false;
}

// ******************************
//
void                                                    DefaultTexturesData::AddFont             ( IFontDescriptorPtr fontDesc )
{
    m_fontDescriptors.push_back( fontDesc );
}

// ******************************
//
void                                                    DefaultTexturesData::SetFont             ( unsigned int idx, IFontDescriptorPtr fontDesc )
{
    if( idx >= m_fontDescriptors.size() )
        m_fontDescriptors.resize( idx + 1 );
    m_fontDescriptors[ idx ] = fontDesc;
}


// ******************************
//
void                                            DefaultTexturesData::UpdateResourceModels	()
{
	for( auto tx : m_textureDescriptors )
	{
		tx->GetSamplerState()->Update();
	}

	for( auto anim : m_animationDescriptors )
	{
		anim->GetSamplerState()->Update();
	}
    for( auto font : m_fontDescriptors )
    {
        font->GetStateModel()->Update();
    }
}

// ******************************
//
void                                            DefaultTexturesData::ClearAll				()
{
    for( auto tx : m_textureDescriptors )
    {
        AssetTracker::Instance().UnregisterAsset( tx );
    }

	m_textureDescriptors.clear();
	m_animationDescriptors.clear();
    m_fontDescriptors.clear();
}

} //model
} //bv
