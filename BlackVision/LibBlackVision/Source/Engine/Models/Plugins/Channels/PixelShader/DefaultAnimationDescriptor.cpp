#include "DefaultAnimationDescriptor.h"

#include "Assets/Texture/TextureLoader.h"
#include "Assets/Texture/AnimationAsset.h"
#include "Assets/AssetManager.h"
#include "Engine/Graphics/Resources/Textures/Texture.h"

#include "Application/ApplicationContext.h"

namespace bv { namespace model {

// *******************************
//
DefaultAnimationDescriptor::DefaultAnimationDescriptor        ()
{
}

// *******************************
//
DefaultAnimationDescriptor::DefaultAnimationDescriptor        ( const std::string & name, UInt32 w, UInt32 h, TextureFormat fmt )
{
    SetName( name );
    SetWidth( w );
    SetHeight( h );
    SetFormat( fmt );
    SetCurrentFrame( 0 );
}

// *******************************
//
DefaultAnimationDescriptor::~DefaultAnimationDescriptor       ()
{
}

// *******************************
//
unsigned int            DefaultAnimationDescriptor::NumTextures         () const
{
    return (unsigned int) m_frames.size();    
}

// *******************************
//
MemoryChunkConstPtr      DefaultAnimationDescriptor::GetBits            ( unsigned int idx ) const
{
    assert( idx < NumTextures() );

	return m_frames[ idx ]->GetOriginal()->GetData();
}

// *******************************
//
UInt64					DefaultAnimationDescriptor::GetUpdateID         () const
{
	return m_updateID;
}

// *******************************
//
unsigned int            DefaultAnimationDescriptor::CurrentFrame        () const
{
    return m_curFrame;
}

// *******************************
//
const std::string       DefaultAnimationDescriptor::GetName             () const
{
    return m_params.GetName();
}

// *******************************
//
UInt32				    DefaultAnimationDescriptor::GetWidth            ( UInt32 level ) const
{
    { level; }
	assert( level == 0 ); // No mipmaps for animation
    return m_params.GetWidth();
}

// *******************************
//
UInt32				    DefaultAnimationDescriptor::GetHeight           ( UInt32 level ) const
{
    { level; }
	assert( level == 0 ); // No mipmaps for animation
    return m_params.GetHeight();
}

// *******************************
//
UInt32	                DefaultAnimationDescriptor::GetDepth            ( UInt32 level ) const
{
    { level; }
	assert( level == 0 ); // No mipmaps for animation
	return 1;
}

// *******************************
//
TextureFormat           DefaultAnimationDescriptor::GetFormat           () const
{
    return m_params.GetFormat();
}

// **************************
//
SamplerStateModelPtr    DefaultAnimationDescriptor::GetSamplerState     () const
{
	return m_params.GetSamplerState();
}

//// *******************************
////
//void                    DefaultAnimationDescriptor::SetBits             ( unsigned int idx, TextureAssetConstPtr texResource )
//{
//    //auto extraKind = handle->GetExtra()->GetResourceExtraKind();
//    //{ extraKind; } // FIXME: suppress unused warning
//    //assert( extraKind == model::ResourceExtraKind::RE_TEXTURE );
//
//    //auto texExtra = static_cast< const model::TextureExtraData * >( handle->GetExtra() );
//    //{ texExtra; } // FIXME: suppress unused warning
//    //assert( texExtra->GetType() == TextureType::T_2D );
//
//	assert( texResource->GetOriginal()->GetWidth() == GetWidth() );
//    assert( texResource->GetOriginal()->GetHeight() == GetHeight() );
//    assert( texResource->GetOriginal()->GetFormat() == GetFormat() );
//    assert( idx < NumTextures() );
//
////    delete[] m_frames[ idx ];
//    
//  //  unsigned int dataSize = GetWidth() * GetHeight() * Texture::GetPixelSize( GetFormat() );
//
//	m_frames[ idx ] = texResource;
////    m_frames[ idx ] = new char[ dataSize ];
//
////    memcpy( m_frames[ idx ], data, dataSize );
//}

// *******************************
//
void                     DefaultAnimationDescriptor::AddBits            ( TextureAssetConstPtr texResource )
{
    //auto extraKind = handle->GetExtra()->GetResourceExtraKind();
    //{ extraKind; } // FIXME: suppress unused warning
    //assert( extraKind == model::ResourceExtraKind::RE_TEXTURE );

    //auto texExtra = static_cast< const model::TextureExtraData * >( handle->GetExtra() );
    //{ texExtra; } // FIXME: suppress unused warning
    //assert( texExtra->GetType() == TextureType::T_2D );

	assert( texResource->GetOriginal()->GetWidth() == GetWidth() );
    assert( texResource->GetOriginal()->GetHeight() == GetHeight() );
    assert( texResource->GetOriginal()->GetFormat() == GetFormat() );

    m_frames.push_back( texResource );

	m_updateID = ApplicationContext::Instance().GetTimestamp() + 1;
}

// *******************************
//
void                     DefaultAnimationDescriptor::SetCurrentFrame     ( unsigned int frameNum )
{
	if( m_curFrame != frameNum )
	{
		m_updateID = ApplicationContext::Instance().GetTimestamp() + 1;
	}
    m_curFrame = frameNum;
}

// *******************************
//
void                    DefaultAnimationDescriptor::SetName             ( const std::string & name )
{
    m_params.SetName( name );
}

// *******************************
//
void                    DefaultAnimationDescriptor::SetWidth            ( unsigned int w )
{
    m_params.SetWidth( w );
}

// *******************************
//
void                    DefaultAnimationDescriptor::SetHeight           ( unsigned int h )
{
    m_params.SetHeight( h );
}

// *******************************
//
void                    DefaultAnimationDescriptor::SetFormat           ( TextureFormat fmt )
{
    m_params.SetFormat( fmt );
}

// **************************
//
void                     DefaultAnimationDescriptor::SetSamplerState	( SamplerStateModelPtr samplerState )
{
	m_params.SetSamplerState( samplerState );
}

// *******************************
//
DefaultAnimationDescriptorPtr DefaultAnimationDescriptor::LoadAnimation  ( const AnimationAssetDescConstPtr & animResDesc, const std::string & name )
{
	auto animRes = LoadTypedAsset<AnimationAsset>( animResDesc );

    if ( animRes == nullptr )
    {
		assert(!"Should never be here");
        return nullptr;
    }

    //auto extraKind = handle->GetExtra()->GetResourceExtraKind();
    //{ extraKind; } // FIXME: suppress unused warning
    //assert( extraKind == model::ResourceExtraKind::RE_TEXTURE );

    //auto texExtra = static_cast< const model::TextureExtraData * >( handle->GetExtra() );
    //{ texExtra; } // FIXME: suppress unused warning
    //assert( texExtra->GetType() == TextureType::T_2D );


	auto texResource = animRes->GetFrame( 0 );

	auto fmt	= texResource->GetOriginal()->GetFormat();
    auto w		= texResource->GetOriginal()->GetWidth();
    auto h		= texResource->GetOriginal()->GetHeight();
	
	auto retDesc = std::make_shared< DefaultAnimationDescriptor >( name, w, h, fmt );

    retDesc->AddBits( texResource );

	for ( SizeType i = 1; i < animRes->GetFramesNum(); ++i )
    {
        auto texResource = animRes->GetFrame( i );

        //auto extraKind = handle->GetExtra()->GetResourceExtraKind();
        //{ extraKind; } // FIXME: suppress unused warning
        //assert( extraKind == model::ResourceExtraKind::RE_TEXTURE );

        //auto texExtra = static_cast< const model::TextureExtraData * >( handle->GetExtra() );
        //assert( texExtra->GetType() == TextureType::T_2D );

        unsigned int lw		= texResource->GetOriginal()->GetWidth();
        unsigned int lh		= texResource->GetOriginal()->GetHeight();
        TextureFormat lfmt	= texResource->GetOriginal()->GetFormat();

        if( lfmt != fmt || lw != w || lh != h )
        {
            retDesc = nullptr;
            break;
        }
        else
        {
            retDesc->AddBits( texResource );
        }
    }

    return retDesc;
}

} //model
} //bv
