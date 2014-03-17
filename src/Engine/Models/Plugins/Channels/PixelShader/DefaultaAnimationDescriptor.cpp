#include "DefaultAnimationDescriptor.h"

#include "Engine/Graphics/Resources/Texture.h"


namespace bv { namespace model {

// **************************
//
DefaultAnimationDescriptor::DefaultAnimationDescriptor        ()
{
}

// **************************
//
DefaultAnimationDescriptor::DefaultAnimationDescriptor        ( const std::string & name, unsigned int w, unsigned int h, TextureFormat fmt, TextureWrappingMode wmx, TextureWrappingMode wmy, TextureFilteringMode fm, const glm::vec4 & bc )
{
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
DefaultAnimationDescriptor::~DefaultAnimationDescriptor       ()
{
    for( auto data : m_frames )
    {
        delete[] data;
    }
}

// **************************
//
unsigned int            DefaultAnimationDescriptor::NumTextures         () const
{
    return m_frames.size();    
}

// **************************
//
const char *            DefaultAnimationDescriptor::GetBits             ( unsigned int idx ) const
{
    assert( idx < NumTextures() );

    return m_frames[ idx ];
}

// **************************
//
const std::string       DefaultAnimationDescriptor::GetName             () const
{
    return m_params.GetName();
}

// **************************
//
unsigned int            DefaultAnimationDescriptor::GetWidth            () const
{
    return m_params.GetWidth();
}

// **************************
//
unsigned int            DefaultAnimationDescriptor::GetHeight           () const
{
    return m_params.GetHeight();
}

// **************************
//
TextureFormat           DefaultAnimationDescriptor::GetFormat           () const
{
    return m_params.GetFormat();
}

// **************************
//
TextureWrappingMode     DefaultAnimationDescriptor::GetWrappingModeX    () const
{
    return m_params.GetWrappingModeX();
}

// **************************
//
TextureWrappingMode     DefaultAnimationDescriptor::GetWrappingModeY    () const
{
    return m_params.GetWrappingModeY();
}

// **************************
//
TextureFilteringMode    DefaultAnimationDescriptor::GetFilteringMode    () const
{
    return m_params.GetFilteringMode();
}

// **************************
//
glm::vec4               DefaultAnimationDescriptor::BorderColor         () const
{
    return m_params.BorderColor();
}

// **************************
//
void                    DefaultAnimationDescriptor::SetBits             ( unsigned int idx, const char * data, TextureFormat fmt, unsigned int w, unsigned int h )
{
    assert( w == GetWidth() );
    assert( h == GetHeight() );
    assert( fmt == GetFormat() );
    assert( idx < NumTextures() );

    delete[] m_frames[ idx ];
    
    unsigned int dataSize = w * h * Texture::GetPixelSize( fmt );

    m_frames[ idx ] = new char[ dataSize ];

    memcpy( m_frames[ idx ], data, dataSize );
}

// **************************
//
void                     DefaultAnimationDescriptor::AddBits            ( const char * data, TextureFormat fmt, unsigned int w, unsigned int h )
{
    assert( w == GetWidth() );
    assert( h == GetHeight() );
    assert( fmt == GetFormat() );

    unsigned int dataSize = w * h * Texture::GetPixelSize( fmt );

    char * newData = new char[ dataSize ];

    memcpy( newData, data, dataSize );

    m_frames.push_back( newData );

}

// **************************
//
void                    DefaultAnimationDescriptor::SetName             ( const std::string & name )
{
    m_params.SetName( name );
}

// **************************
//
void                    DefaultAnimationDescriptor::SetWidth            ( unsigned int w )
{
    m_params.SetWidth( w );
}

// **************************
//
void                    DefaultAnimationDescriptor::SetHeight           ( unsigned int h )
{
    m_params.SetHeight( h );
}

// **************************
//
void                    DefaultAnimationDescriptor::SetFormat           ( TextureFormat fmt )
{
    m_params.SetFormat( fmt );
}

// **************************
//
void                    DefaultAnimationDescriptor::SetWrappingModeX    ( TextureWrappingMode wm )
{
    m_params.SetWrappingModeX( wm );
}

// **************************
//
void                    DefaultAnimationDescriptor::SetWrappingModeY    ( TextureWrappingMode wm )
{
    m_params.SetWrappingModeY( wm );
}

// **************************
//
void                    DefaultAnimationDescriptor::SetFilteringMode    ( TextureFilteringMode fm )
{
    m_params.SetFilteringMode( fm );
}

// **************************
//
void                    DefaultAnimationDescriptor::SetBorderColor      ( const glm::vec4 & bc )
{
    m_params.SetBorderColor( bc );
}

} //model
} //bv
