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
    for( auto tx : m_textureDescriptors )
    {
        delete tx;
    }

    for( auto anim : m_animationDescriptors )
    {
        delete anim;
    }
}

// ******************************
//
const std::vector< ITextureDescriptor * > &     DefaultTexturesData::GetTextures        () const
{
    return m_textureDescriptors;
}

// ******************************
//
const std::vector< IAnimationDescriptor * > &   DefaultTexturesData::GetAnimations      () const
{
    return m_animationDescriptors;
}

// ******************************
//
ITextureDescriptor *                            DefaultTexturesData::GetTexture          ( unsigned int idx )
{
    assert( idx < m_textureDescriptors.size() );

    return m_textureDescriptors[ idx ];
}

// ******************************
//
IAnimationDescriptor *                          DefaultTexturesData::GetAnimation        ( unsigned int idx )
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
bool                                            DefaultTexturesData::SetTexture         ( unsigned int idx, ITextureDescriptor * textureDesc )
{
    if( idx < m_textureDescriptors.size() )
	{
		delete m_textureDescriptors[ idx ];
		m_textureDescriptors[ idx ] = textureDesc;
		return true;
	}
	return false;
}

// ******************************
//
void                                            DefaultTexturesData::AddTexture         ( ITextureDescriptor * textureDesc )
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
bool                                            DefaultTexturesData::SetAnimation       ( unsigned int idx, IAnimationDescriptor * animationDesc )
{
	if( idx < m_animationDescriptors.size() )
	{
		delete m_animationDescriptors[ idx ];
		m_animationDescriptors[ idx ] = animationDesc;
		return true;
	}
	return false;
}

// ******************************
//
void                                            DefaultTexturesData::AddAnimation       ( IAnimationDescriptor * animationDesc )
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
		auto desc = static_cast< DefaultAnimationDescriptor * >( m_animationDescriptors[ idx ] );
		desc->SetCurrentFrame( frameNum );
		return true;
	}
    return false;
}

} //model
} //bv
