#include "DefaultTextureParams.h"

namespace bv { namespace model {

// **************************
//
DefaultTextureParams::DefaultTextureParams ()
{
}

// **************************
//
DefaultTextureParams::DefaultTextureParams ( const std::string & name, unsigned int w, unsigned int h, TextureFormat fmt, TextureWrappingMode wmx, TextureWrappingMode wmy, TextureFilteringMode fm, const glm::vec4 & bc )
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
DefaultTextureParams::~DefaultTextureParams()
{
}

// **************************
//
const std::string       DefaultTextureParams::GetName           () const
{
    return m_name;
}

// **************************
//
unsigned int            DefaultTextureParams::GetWidth          () const
{
    return m_width;
}

// **************************
//
unsigned int            DefaultTextureParams::GetHeight         () const
{
    return m_height;
}

// **************************
//
TextureFormat           DefaultTextureParams::GetFormat         () const
{
    return m_format;
}

// **************************
//
TextureWrappingMode     DefaultTextureParams::GetWrappingModeX  () const
{
    return m_wrappingModeX;
}

// **************************
//
TextureWrappingMode     DefaultTextureParams::GetWrappingModeY  () const
{
    return m_wrappingModeY;
}

// **************************
//
TextureFilteringMode    DefaultTextureParams::GetFilteringMode  () const
{
    return m_filteringMode;
}

// **************************
//
glm::vec4               DefaultTextureParams::BorderColor       () const
{
    return m_borderColor;
}

// **************************
//
void                    DefaultTextureParams::SetName           ( const std::string & name )
{
    m_name = name;
}
// **************************
//
void                    DefaultTextureParams::SetWidth          ( unsigned int w )
{
    m_width = w;
}

// **************************
//
void                    DefaultTextureParams::SetHeight         ( unsigned int h )
{
    m_height = h;
}

// **************************
//
void                    DefaultTextureParams::SetFormat         ( TextureFormat fmt )
{
    m_format = fmt;
}

// **************************
//
void                    DefaultTextureParams::SetWrappingModeX  ( TextureWrappingMode wm )
{
    m_wrappingModeX = wm;
}

// **************************
//
void                    DefaultTextureParams::SetWrappingModeY  ( TextureWrappingMode wm )
{
    m_wrappingModeY = wm;
}

// **************************
//
void                    DefaultTextureParams::SetFilteringMode  ( TextureFilteringMode fm )
{
    m_filteringMode = fm;
}

// **************************
//
void                    DefaultTextureParams::SetBorderColor    ( const glm::vec4 & bc )
{
    m_borderColor = bc;
}

} //model
} //bv
