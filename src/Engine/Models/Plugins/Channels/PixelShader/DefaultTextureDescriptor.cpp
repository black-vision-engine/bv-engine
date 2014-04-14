#include "DefaultTextureDescriptor.h"

#include "Engine/Models/Resources/TextureLoader.h"

#include "Engine/Graphics/Resources/Texture.h"


namespace bv { namespace model {

// **************************
//
DefaultTextureDescriptor::DefaultTextureDescriptor        ()
    : m_data( nullptr )
{
}

// **************************
//
DefaultTextureDescriptor::DefaultTextureDescriptor        ( const char * data, const std::string & name, unsigned int w, unsigned int h, TextureFormat fmt, TextureWrappingMode wmx, TextureWrappingMode wmy, TextureFilteringMode fm, const glm::vec4 & bc )
    : m_data( nullptr )
{
    SetBits( data, fmt, w, h );
    SetName( name );
    SetWidth( w );
    SetHeight( h );
    SetFormat( fmt );
    SetWrappingModeX( wmx );
    SetWrappingModeY( wmy );
    SetFilteringMode( fm );
    SetBorderColor( bc );
}

// **************************
//
DefaultTextureDescriptor::~DefaultTextureDescriptor       ()
{
    delete[] m_data;
}

// **************************
//
const char *            DefaultTextureDescriptor::GetBits           () const
{
    return m_data;
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
void                    DefaultTextureDescriptor::SetBits           ( const char * data, TextureFormat fmt, unsigned int w, unsigned int h )
{
    delete[] m_data;

    m_params.SetWidth( w );
    m_params.SetHeight( h );
    
    unsigned int dataSize = w * h * Texture::GetPixelSize( fmt );

    m_data = new char[ dataSize ];

    memcpy( m_data, data, dataSize );
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
    desc->SetBits( nullptr, TextureFormat::F_A8R8G8B8, 0, 0 );
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
    Resource texture( name, textureFile );

    TextureLoader texLoader;
    ResourceHandle * handle = texLoader.LoadResource( &texture );

    if ( handle == nullptr )
    {
        return nullptr;
    }

    auto extraKind = handle->GetExtra()->GetResourceExtraKind();
    assert( extraKind == model::ResourceExtraKind::RE_TEXTURE );

    auto texExtra = static_cast< const model::TextureExtraData * >( handle->GetExtra() );
    assert( texExtra->GetType() == TextureType::T_2D );

    auto format = texExtra->GetFormat();
    auto width  = texExtra->GetWidth();
    auto height = texExtra->GetHeight();

    DefaultTextureDescriptor * desc = new DefaultTextureDescriptor();
    SetDefaults( desc );

    desc->SetBits( handle->GetData(), format, width, height );
    desc->SetName( name );

    delete handle;

    return desc;
}

} //model
} //bv
