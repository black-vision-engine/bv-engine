#include "DefaultTexturesData.h"


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
void                                            DefaultTexturesData::SetTextureParams   ( unsigned int idx, TextureWrappingMode wrapModeX, TextureWrappingMode wrapModeY, TextureFilteringMode filteringMode, const glm::vec4 & borderColor )
{
    assert( idx < m_textureDescriptors.size() );

	auto samplerState = m_textureDescriptors[ idx ]->GetSamplerState();
    samplerState->SetWrappingModeX( wrapModeX );
    samplerState->SetWrappingModeY( wrapModeY );
    samplerState->SetFilteringMode( filteringMode );
    samplerState->SetBorderColor( borderColor );
}

// ******************************
//
void                                            DefaultTexturesData::SetTexture         ( unsigned int idx, ITextureDescriptorPtr textureDesc )
{
    if( idx >= m_textureDescriptors.size() )
	{
		m_textureDescriptors.resize( idx + 1 );
	} 
	m_textureDescriptors[ idx ] = textureDesc;
}

// ******************************
//
void                                            DefaultTexturesData::AddTexture         ( ITextureDescriptorPtr textureDesc )
{
    m_textureDescriptors.push_back( textureDesc );
}

// ******************************
//
void                                            DefaultTexturesData::SetAnimationParams ( unsigned int idx, TextureWrappingMode wrapModeX, TextureWrappingMode wrapModeY, TextureFilteringMode filteringMode, const glm::vec4 & borderColor )
{
    assert( idx < m_animationDescriptors.size() );

	auto samplerState = m_animationDescriptors[ idx ]->GetSamplerState();
    samplerState->SetWrappingModeX( wrapModeX );
    samplerState->SetWrappingModeY( wrapModeY );
    samplerState->SetFilteringMode( filteringMode );
    samplerState->SetBorderColor( borderColor );
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
}

// ******************************
//
void                                            DefaultTexturesData::ClearAll				()
{
	m_textureDescriptors.clear();
	m_animationDescriptors.clear();
}

} //model
} //bv
