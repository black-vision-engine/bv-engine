#include "DefaultTextureDescriptor.h"

#include "Engine/Models/Resources/ModelTextureManager"
#include "Engine/Models/Resources/TextureLoader.h"
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
DefaultTextureDescriptor::DefaultTextureDescriptor        ( ResourceHandleConstPtr handle, const std::string & name, TextureWrappingMode wmx, TextureWrappingMode wmy, TextureFilteringMode fm, const glm::vec4 & bc )
    : m_bitsChanged( true )
{
    SetBits( handle );
    SetName( name );

    auto extraKind = handle->GetExtra()->GetResourceExtraKind();
    assert( extraKind == model::ResourceExtraKind::RE_TEXTURE );

    auto texExtra = static_cast< const model::TextureExtraData * >( handle->GetExtra() );
    assert( texExtra->GetType() == TextureType::T_2D );

    auto format = texExtra->GetFormat();
    auto width  = texExtra->GetWidth();
    auto height = texExtra->GetHeight();

    SetWidth( width );
    SetHeight( height );
    SetFormat( format );
    SetWrappingModeX( wmx );
    SetWrappingModeY( wmy );
    SetFilteringMode( fm );
    SetBorderColor( bc );
}

// **************************
//
DefaultTextureDescriptor::~DefaultTextureDescriptor       ()
{
}

// **************************
//
const char *            DefaultTextureDescriptor::GetBits           () const
{
    return m_texHandle->GetData();
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
unsigned int            DefaultTextureDescriptor::GetWidth          () const
{
    return m_params.GetWidth();
}

// **************************
//
unsigned int            DefaultTextureDescriptor::GetHeight         () const
{
    return m_params.GetHeight();
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
void                    DefaultTextureDescriptor::SetBits           ( ResourceHandleConstPtr handle )
{
    if( handle == nullptr )
    {
        m_params.SetWidth( 0 );
        m_params.SetHeight( 0 );
    }
    else
    {
        auto extraKind = handle->GetExtra()->GetResourceExtraKind();
        assert( extraKind == model::ResourceExtraKind::RE_TEXTURE );

        auto texExtra = static_cast< const model::TextureExtraData * >( handle->GetExtra() );
        assert( texExtra->GetType() == TextureType::T_2D );

        auto fmt = texExtra->GetFormat();
        auto w  = texExtra->GetWidth();
        auto h = texExtra->GetHeight();

        m_params.SetWidth( w );
        m_params.SetHeight( h );
    }

    m_texHandle = handle;

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
void                    DefaultTextureDescriptor::SetWidth          ( unsigned int w )
{
    m_params.SetWidth( w );
}

// **************************
//
void                    DefaultTextureDescriptor::SetHeight         ( unsigned int h )
{
    m_params.SetHeight( h );
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
void                        DefaultTextureDescriptor::SetDefaults     ( DefaultTextureDescriptor * desc )
{
    desc->SetBits( nullptr );
    desc->SetName( "" );
    desc->SetWidth( 0 );
    desc->SetHeight( 0 );
    desc->SetFormat( TextureFormat::F_A8R8G8B8 );
    desc->SetWrappingModeX( TextureWrappingMode::TWM_REPEAT );
    desc->SetWrappingModeY( TextureWrappingMode::TWM_REPEAT );
    desc->SetFilteringMode( TextureFilteringMode::TFM_LINEAR );
    desc->SetBorderColor( glm::vec4( 0.f, 0.f, 0.f, 1.f ) );
}

// **************************
//
DefaultTextureDescriptor *  DefaultTextureDescriptor::LoadTexture    ( const std::string & textureFile, const std::string & name )
{
    auto handle = TextureManager::Get().GetTexture( textureFile );

    if ( handle == nullptr )
    {
        return nullptr;
    }

    DefaultTextureDescriptor * desc = new DefaultTextureDescriptor();
    SetDefaults( desc );

    desc->SetBits( handle );
    desc->SetName( name );

    return desc;
}

} //model
} //bv
