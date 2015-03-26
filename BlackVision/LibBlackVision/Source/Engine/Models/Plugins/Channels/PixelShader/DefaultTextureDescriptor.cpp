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
DefaultTextureDescriptor::DefaultTextureDescriptor        ( TextureAssetConstPtr texResource, const std::string & name, TextureWrappingMode wmx, TextureWrappingMode wmy, TextureFilteringMode fm, const glm::vec4 & bc, DataBuffer::Semantic semantic )
    : m_bitsChanged( true )
{
    SetBits( texResource );
    SetName( name );

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
    SetWrappingModeX( wmx );
    SetWrappingModeY( wmy );
    SetFilteringMode( fm );
    SetBorderColor( bc );
    SetSemantic( semantic );
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
SizeType				DefaultTextureDescriptor::GetNumLevels		() const
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

	SizeType numLevel = 1;

	if( m_texResource->GetMipMaps() != nullptr )
	{
		numLevel += m_texResource->GetMipMaps()->GetLevelsNum();
	}

	res.reserve( numLevel );

	res.push_back( m_texResource->GetOriginal()->GetData() );

	for( UInt32 i = 1; i < numLevel; ++i )
	{
		res.push_back( m_texResource->GetMipMaps()->GetLevel( i - 1 )->GetData() );
	}

	return res;
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
SizeType				DefaultTextureDescriptor::GetWidth          ( UInt32 level ) const
{
    return m_params.GetWidth() >> level;
}

// **************************
//
SizeType				DefaultTextureDescriptor::GetHeight         ( UInt32 level ) const
{
    return m_params.GetHeight() >> level;
}

// **************************
//
SizeType				DefaultTextureDescriptor::GetDepth          ( UInt32 level ) const
{
    return m_params.GetDepth() >> level;
}

// **************************
//
TextureFormat           DefaultTextureDescriptor::GetFormat         () const
{
    return m_params.GetFormat();
}

// **************************
//
TextureWrappingMode     DefaultTextureDescriptor::GetWrappingModeX  () const
{
    return m_params.GetWrappingModeX();
}

// **************************
//
TextureWrappingMode     DefaultTextureDescriptor::GetWrappingModeY  () const
{
    return m_params.GetWrappingModeY();
}

// **************************
//
TextureWrappingMode     DefaultTextureDescriptor::GetWrappingModeZ  () const
{
    return m_params.GetWrappingModeY();
}

// **************************
//
TextureFilteringMode    DefaultTextureDescriptor::GetFilteringMode  () const
{
    return m_params.GetFilteringMode();
}

// **************************
//
glm::vec4               DefaultTextureDescriptor::BorderColor       () const
{
    return m_params.BorderColor();
}

// **************************
//
DataBuffer::Semantic    DefaultTextureDescriptor::GetSemantic     () const
{
    return m_semantic;
}

// **************************
//
void                    DefaultTextureDescriptor::SetBits           ( TextureAssetConstPtr texResource )
{
    if( texResource == nullptr )
    {
        m_params.SetWidth( 0 );
        m_params.SetHeight( 0 );
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
		auto w  = texResource->GetOriginal()->GetWidth();
        auto h  = texResource->GetOriginal()->GetHeight();

        m_params.SetWidth( w );
        m_params.SetHeight( h );
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
void                    DefaultTextureDescriptor::SetWidth          ( SizeType w )
{
    m_params.SetWidth( w );
}

// **************************
//
void                    DefaultTextureDescriptor::SetHeight         ( SizeType h )
{
    m_params.SetHeight( h );
}

// **************************
//
void                    DefaultTextureDescriptor::SetDepth          ( SizeType d )
{
    m_params.SetDepth( d );
}

// **************************
//
void                    DefaultTextureDescriptor::SetFormat         ( TextureFormat fmt )
{
    m_params.SetFormat( fmt );
}

// **************************
//
void                    DefaultTextureDescriptor::SetWrappingModeX  ( TextureWrappingMode wm )
{
    m_params.SetWrappingModeX( wm );
}

// **************************
//
void                    DefaultTextureDescriptor::SetWrappingModeY  ( TextureWrappingMode wm )
{
    m_params.SetWrappingModeY( wm );
}

// **************************
//
void                    DefaultTextureDescriptor::SetFilteringMode  ( TextureFilteringMode fm )
{
    m_params.SetFilteringMode( fm );
}

// **************************
//
void                    DefaultTextureDescriptor::SetBorderColor    ( const glm::vec4 & bc )
{
    m_params.SetBorderColor( bc );
}

// **************************
//
void                        DefaultTextureDescriptor::SetSemantic     ( DataBuffer::Semantic semantic )
{
    m_semantic = semantic;
}

// **************************
//
void                        DefaultTextureDescriptor::SetDefaults     ( DefaultTextureDescriptor * desc )
{
    desc->SetBits( nullptr );
    desc->SetName( "" );
    desc->SetWidth( 0 );
    desc->SetHeight( 0 );
    desc->SetFormat( TextureFormat::F_A8R8G8B8 );
    desc->SetWrappingModeX( TextureWrappingMode::TWM_CLAMP_BORDER );
    desc->SetWrappingModeY( TextureWrappingMode::TWM_CLAMP_BORDER );
    desc->SetFilteringMode( TextureFilteringMode::TFM_LINEAR );
    desc->SetBorderColor( glm::vec4( 0.f, 0.f, 0.f, 0.f ) );
}

// **************************
//
DefaultTextureDescriptor *  DefaultTextureDescriptor::LoadTexture    ( const TextureAssetDescConstPtr & textureResDesc, const std::string & name )
{
	auto res = AssetManager::GetInstance().LoadAsset( textureResDesc );

    if ( res == nullptr )
    {
        return nullptr;
    }

    DefaultTextureDescriptor * desc = new DefaultTextureDescriptor();
    SetDefaults( desc );
    //desc->SetWrappingModeY( TextureWrappingMode::TWM_REPEAT ); 
    //desc->SetFilteringMode( TextureFilteringMode::TFM_POINT ); 
	desc->SetBits( QueryTypedRes< TextureAssetConstPtr >( res ) );
    desc->SetName( name );

    return desc;
}

} //model
} //bv
