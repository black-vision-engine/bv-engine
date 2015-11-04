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
void                                            DefaultTexturesData::SetTextureParams   ( unsigned int idx, TextureWrappingMode wrapModeX, TextureWrappingMode wrapModeY, TextureFilteringMode filteringMode, const glm::vec4 & borderColor )
{
    assert( idx < m_textureDescriptors.size() );

    auto desc = static_cast< DefaultTextureDescriptor * >( m_textureDescriptors[ idx ] );

    desc->SetWrappingModeX( wrapModeX );
    desc->SetWrappingModeY( wrapModeY );
    desc->SetFilteringMode( filteringMode );
    desc->SetBorderColor( borderColor );
}

// ******************************
//
bool                                            DefaultTexturesData::SetTexture         ( unsigned int idx, ITextureDescriptorPtr textureDesc )
{
    if( idx < m_textureDescriptors.size() )
	{
		m_textureDescriptors[ idx ] = textureDesc;
		return true;
	}
	return false;
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

    auto desc = static_cast< DefaultAnimationDescriptor * >( m_animationDescriptors[ idx ] );

    desc->SetWrappingModeX( wrapModeX );
    desc->SetWrappingModeY( wrapModeY );
    desc->SetFilteringMode( filteringMode );
    desc->SetBorderColor( borderColor );
}

// ******************************
//
bool                                            DefaultTexturesData::SetAnimation       ( unsigned int idx, IAnimationDescriptorPtr animationDesc )
{
	if( idx < m_animationDescriptors.size() )
	{
		m_animationDescriptors[ idx ] = animationDesc;
		return true;
	}
	return false;
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

} //model
} //bv
