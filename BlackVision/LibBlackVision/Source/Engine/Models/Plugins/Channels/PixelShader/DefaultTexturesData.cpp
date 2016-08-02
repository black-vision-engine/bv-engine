#include "stdafx.h"

#include "DefaultTexturesData.h"

#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureDescriptor.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultAnimationDescriptor.h"

#include "Engine/Models/AssetTracker.h"




#include "Memory/MemoryLeaks.h"



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
        TriggerEvent( AssetTrackerInternalEvent::Command::UnregisterAsset, m_textureDescriptors[ idx ] );
    }

	m_textureDescriptors[ idx ] = textureDesc;

    TriggerEvent( AssetTrackerInternalEvent::Command::RegisterAsset, textureDesc );
}

// ******************************
//
void                                            DefaultTexturesData::AddTexture         ( ITextureDescriptorPtr textureDesc )
{
    m_textureDescriptors.push_back( textureDesc );

    auto assetTrackerEvt = std::make_shared< AssetTrackerInternalEvent >( AssetTrackerInternalEvent::Command::RegisterAsset );
    assetTrackerEvt->TextureAsset = textureDesc;
    GetDefaultEventManager().TriggerEvent( assetTrackerEvt );

    TriggerEvent( AssetTrackerInternalEvent::Command::RegisterAsset, textureDesc );
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
        TriggerEvent( AssetTrackerInternalEvent::Command::UnregisterAsset, tx );
    }

	m_textureDescriptors.clear();
	m_animationDescriptors.clear();
    m_fontDescriptors.clear();
}

// ******************************
//
void                                            DefaultTexturesData::TriggerEvent	        ( AssetTrackerInternalEvent::Command command, ITextureDescriptorPtr textureDesc )
{
    auto evt = std::make_shared< AssetTrackerInternalEvent >( command );
    evt->TextureAsset = textureDesc;
    GetDefaultEventManager().TriggerEvent( evt );
}

} //model
} //bv
