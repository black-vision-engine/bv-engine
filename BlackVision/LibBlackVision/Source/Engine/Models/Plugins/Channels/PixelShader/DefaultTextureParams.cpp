#include "DefaultTextureParams.h"

namespace bv { namespace model {

// **************************
//
DefaultTextureParams::DefaultTextureParams ()
{
}

// **************************
//
DefaultTextureParams::DefaultTextureParams ( const std::string & name, UInt32 w, UInt32 h, UInt32 z, TextureFormat fmt, TextureWrappingMode wmx, TextureWrappingMode wmy, TextureWrappingMode wmz, TextureFilteringMode fm, const glm::vec4 & bc )
{
    SetName( name );
    SetWidth( w );
    SetHeight( h );
	SetDepth( z );
    SetFormat( fmt );
    SetWrappingModeX( wmx );
    SetWrappingModeY( wmy );
	SetWrappingModeZ( wmz );
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
UInt32				    DefaultTextureParams::GetWidth          ( UInt32 level ) const
{
    return m_width >> level;
}

// **************************
//
UInt32				    DefaultTextureParams::GetHeight         ( UInt32 level ) const
{
    return m_height >> level;
}

// **************************
//
UInt32				    DefaultTextureParams::GetDepth          ( UInt32 level ) const
{
    return m_depth >> level;
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
TextureWrappingMode     DefaultTextureParams::GetWrappingModeZ  () const
{
    return m_wrappingModeZ;
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
void                    DefaultTextureParams::SetWidth          ( UInt32 w )
{
    m_width = w;
}

// **************************
//
void                    DefaultTextureParams::SetHeight         ( UInt32 h )
{
    m_height = h;
}

// **************************
//
void                    DefaultTextureParams::SetDepth          ( UInt32 d )
{
    m_depth = d;
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
void                    DefaultTextureParams::SetWrappingModeZ  ( TextureWrappingMode wm )
{
    m_wrappingModeZ = wm;
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
