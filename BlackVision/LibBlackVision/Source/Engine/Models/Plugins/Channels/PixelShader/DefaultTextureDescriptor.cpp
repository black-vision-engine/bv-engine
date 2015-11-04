#include "DefaultTextureDescriptor.h"

#include "Assets/AssetManager.h"
#include "Assets/Texture/TextureLoader.h"
#include "Engine/Graphics/Resources/Texture.h"


namespace bv { namespace model {

// **************************
//
DefaultTextureDescriptor::DefaultTextureDescriptor        ()
    : m_bitsChanged( false )
{
}

// **************************
//
DefaultTextureDescriptor::DefaultTextureDescriptor        ( TextureAssetConstPtr texResource, const std::string & name, DataBuffer::Semantic semantic )
    : m_bitsChanged( true )
{
    //auto extraKind = handle->GetExtra()->GetResourceExtraKind();
    //{ extraKind; } // FIXME: suppress unused warning
    //assert( extraKind == model::ResourceExtraKind::RE_TEXTURE );

    //auto texExtra = static_cast< const model::TextureExtraData * >( handle->GetExtra() );
    //{ texExtra; } // FIXME: suppress unused warning
    //assert( texExtra->GetType() == TextureType::T_2D );

	auto format = texResource->GetOriginal()->GetFormat();
    auto width  = texResource->GetOriginal()->GetWidth();
    auto height = texResource->GetOriginal()->GetHeight();

    SetWidth( width );
    SetHeight( height );
	SetDepth( 1 );
    SetFormat( format );
    SetSemantic( semantic );

	SetBits( texResource );
    SetName( name );
}

// **************************
//
DefaultTextureDescriptor::~DefaultTextureDescriptor                 ()
{
}

// **************************
//
uintptr_t               DefaultTextureDescriptor::GetUID            () const
{
    return (uintptr_t) GetBits( 0 )->Get();
}

// **************************
//
UInt32				    DefaultTextureDescriptor::GetNumLevels		() const
{
	if( m_texResource->GetMipMaps() != nullptr )
	{
		return ( UInt32 )m_texResource->GetMipMaps()->GetLevelsNum() + 1;
	}
	else
	{
		return 1;
	}
}

// **************************
//
MemoryChunkConstPtr     DefaultTextureDescriptor::GetBits           ( UInt32 level ) const
{
	if( level == 0 )
	{
		return m_texResource->GetOriginal()->GetData();
	}
	else
	{
		assert( level < m_texResource->GetMipMaps()->GetLevelsNum() );
		return m_texResource->GetMipMaps()->GetLevel( level )->GetData();
	}
}

// **************************
//
MemoryChunkVector		DefaultTextureDescriptor::GetBits			() const
{
	MemoryChunkVector res;

	if( !m_texResource->HasMipMaps() )
	{
		res.push_back( m_texResource->GetOriginal()->GetData() );
		return res;
	}
	else
	{
		SizeType numLevel = m_texResource->GetMipMaps()->GetLevelsNum();

		for( UInt32 i = 0; i < numLevel; ++i )
		{
			res.push_back( m_texResource->GetMipMaps()->GetLevel( i )->GetData() );
		}

		return res;
	}
}

// **************************
//
bool                    DefaultTextureDescriptor::BitsChanged       () const
{
    return m_bitsChanged;
}

// **************************
//
void                    DefaultTextureDescriptor::ResetBitsChanged  () const
{
    SetBitsChanged( false );
}

// **************************
//
const std::string       DefaultTextureDescriptor::GetName           () const
{
    return m_params.GetName();
}

// **************************
//
UInt32				    DefaultTextureDescriptor::GetWidth          ( UInt32 level ) const
{
    return m_params.GetWidth() >> level;
}

// **************************
//
UInt32				    DefaultTextureDescriptor::GetHeight         ( UInt32 level ) const
{
    return m_params.GetHeight() >> level;
}

// **************************
//
UInt32				    DefaultTextureDescriptor::GetDepth          ( UInt32 level ) const
{
    return m_params.GetDepth() >> level;
}

// **************************
//
TextureFormat           DefaultTextureDescriptor::GetFormat         () const
{
    return m_params.GetFormat();
}

//// **************************
////
//TextureWrappingMode     DefaultTextureDescriptor::GetWrappingModeX  () const
//{
//    return m_params.GetWrappingModeX();
//}
//
//// **************************
////
//TextureWrappingMode     DefaultTextureDescriptor::GetWrappingModeY  () const
//{
//    return m_params.GetWrappingModeY();
//}
//
//// **************************
////
//TextureWrappingMode     DefaultTextureDescriptor::GetWrappingModeZ  () const
//{
//    return m_params.GetWrappingModeZ();
//}
//
//// **************************
////
//TextureFilteringMode    DefaultTextureDescriptor::GetFilteringMode  () const
//{
//    return m_params.GetFilteringMode();
//}
//
//// **************************
////
//glm::vec4               DefaultTextureDescriptor::BorderColor       () const
//{
//    return m_params.BorderColor();
//}

// **************************
//
DataBuffer::Semantic    DefaultTextureDescriptor::GetSemantic     () const
{
    return m_semantic;
}

// **************************
//
SamplerStateModelPtr    DefaultTextureDescriptor::GetSamplerState     () const
{
	return m_params.GetSamplerState();
}

// **************************
//
void                    DefaultTextureDescriptor::SetBits           ( TextureAssetConstPtr texResource )
{
    if( texResource == nullptr )
    {
        m_params.SetWidth( 0 );
        m_params.SetHeight( 0 );
		m_params.SetDepth( 0 );
    }
    else
    {
        //auto extraKind = handle->GetExtra()->GetResourceExtraKind();
        //{ extraKind; } // FIXME: suppress unused warning
        //assert( extraKind == model::ResourceExtraKind::RE_TEXTURE );

        //auto texExtra = static_cast< const model::TextureExtraData * >( handle->GetExtra() );
        //{ texExtra; } // FIXME: suppress unused warning
        //assert( texExtra->GetType() == TextureType::T_2D );

        //auto fmt = texExtra->GetFormat();
		auto w  = 0;
        auto h  = 0;

		auto mm = texResource->GetMipMaps();

		if( texResource->HasMipMaps() )
		{
			w = mm->GetLevel( 0 )->GetWidth();
			h = mm->GetLevel( 0 )->GetHeight();
		}
		else
		{
			w = texResource->GetOriginal()->GetWidth();
			h = texResource->GetOriginal()->GetHeight();
		}

        m_params.SetWidth( w );
        m_params.SetHeight( h );
		m_params.SetDepth( 1 );
    }

    m_texResource = texResource;

    SetBitsChanged( true );
}

// **************************
//
void                    DefaultTextureDescriptor::SetBitsChanged    ( bool bitsChanged ) const
{
    m_bitsChanged = bitsChanged;
}

// **************************
//
void                    DefaultTextureDescriptor::SetName           ( const std::string & name )
{
    m_params.SetName( name );
}

// **************************
//
void                    DefaultTextureDescriptor::SetWidth          ( UInt32 w )
{
    m_params.SetWidth( w );
}

// **************************
//
void                    DefaultTextureDescriptor::SetHeight         ( UInt32 h )
{
    m_params.SetHeight( h );
}

// **************************
//
void                    DefaultTextureDescriptor::SetDepth          ( UInt32 d )
{
    m_params.SetDepth( d );
}

// **************************
//
void                    DefaultTextureDescriptor::SetFormat         ( TextureFormat fmt )
{
    m_params.SetFormat( fmt );
}

//// **************************
////
//void                    DefaultTextureDescriptor::SetWrappingModeX  ( TextureWrappingMode wm )
//{
//	m_params.GetSamplerState()->SetWrappingModeX( wm );
//}
//
//// **************************
////
//void                    DefaultTextureDescriptor::SetWrappingModeY  ( TextureWrappingMode wm )
//{
//    m_params.GetSamplerState()->SetWrappingModeY( wm );
//}
//
//// **************************
////
//void                    DefaultTextureDescriptor::SetWrappingModeZ  ( TextureWrappingMode wm )
//{
//    m_params.GetSamplerState()->SetWrappingModeZ( wm );
//}
//
//// **************************
////
//void                    DefaultTextureDescriptor::SetFilteringMode  ( TextureFilteringMode fm )
//{
//    m_params.GetSamplerState()->SetFilteringMode( fm );
//}
//
//// **************************
////
//void                    DefaultTextureDescriptor::SetBorderColor    ( const glm::vec4 & bc )
//{
//    m_params.GetSamplerState()->SetBorderColor( bc );
//}

// **************************
//
void                        DefaultTextureDescriptor::SetSemantic     ( DataBuffer::Semantic semantic )
{
    m_semantic = semantic;
}

// **************************
//
void                        DefaultTextureDescriptor::SetSamplerState   ( SamplerStateModelPtr samplerState )
{
	m_params.SetSamplerState( samplerState );
}

// **************************
//
void                        DefaultTextureDescriptor::SetDefaults     ( DefaultTextureDescriptorPtr desc )
{
    desc->SetBits( nullptr );
    desc->SetName( "" );
    desc->SetWidth( 0 );
    desc->SetHeight( 0 );
	desc->SetDepth( 0 );
    desc->SetFormat( TextureFormat::F_A8R8G8B8 );
}

// **************************
//
DefaultTextureDescriptorPtr  DefaultTextureDescriptor::LoadTexture    ( const TextureAssetDescConstPtr & textureResDesc, const std::string & name )
{
	auto res = LoadTypedAsset<TextureAsset>( textureResDesc );

    if ( res == nullptr )
    {
        return nullptr;
    }

	auto desc = std::make_shared< DefaultTextureDescriptor >();
    SetDefaults( desc );

	desc->SetBits( res );
    desc->SetName( name );
	desc->SetFormat( res->GetOriginal()->GetFormat() );

    return desc;
}

} //model
} //bv