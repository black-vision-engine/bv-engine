#include "DefaultTextureParams.h"

namespace bv { namespace model {

// **************************
//
DefaultTexturesData::DefaultTexturesData ()
{
}

// **************************
//
DefaultTexturesData::DefaultTexturesData ( const std::string & name, unsigned int w, unsigned int h, TextureWrappingMode wmx, TextureWrappingMode wmy, TextureFilteringMode fm, const glm::vec4 & bc )
{
    SetName( name );
    SetWidth( w );
    SetHeight( h );
    SetWrappingModeX( wmx );
    SetWrappingModeY( wmy );
    SetFilteringMode( fm );
    SetBorderColor( bc );
}

// **************************
//
DefaultTexturesData::~DefaultTexturesData()
{
}

// **************************
//
const std::string       DefaultTexturesData::GetName            () const
{
    return m_name;
}

// **************************
//
unsigned int            DefaultTexturesData::GetWidth           () const
{
    return m_width;
}

// **************************
//
unsigned int            DefaultTexturesData::GetHeight          () const
{
    return m_height;
}

// **************************
//
TextureFormat           DefaultTexturesData::GetFormat          () const
{
    return m_format;
}

// **************************
//
TextureWrappingMode     DefaultTexturesData::GetWrappingModeX   () const
{
    return m_wrappingModeX;
}

// **************************
//
TextureWrappingMode     DefaultTexturesData::GetWrappingModeY   () const
{
    return m_wrappingModeY;
}

// **************************
//
TextureFilteringMode    DefaultTexturesData::GetFilteringMode   () const
{
    return m_filteringMode;
}

// **************************
//
glm::vec4               DefaultTexturesData::BorderColor        () const
{
    return m_borderColor;
}

// **************************
//
void                    DefaultTexturesData::SetName            ( const std::string & name )
{
    m_name = name;
}
// **************************
//
void                    DefaultTexturesData::SetWidth           ( unsigned int w )
{
    m_width = w;
}

// **************************
//
void                    DefaultTexturesData::SetHeight          ( unsigned int h )
{
    m_height = h;
}

// **************************
//
void                    DefaultTexturesData::SetFormat          ( TextureFormat format )
{
    m_format = format;
}

// **************************
//
void                    DefaultTexturesData::SetWrappingModeX   ( TextureWrappingMode wm )
{
    m_wrappingModeX = wm;
}

// **************************
//
void                    DefaultTexturesData::SetWrappingModeY   ( TextureWrappingMode wm )
{
    m_wrappingModeY = wm;
}

// **************************
//
void                    DefaultTexturesData::SetFilteringMode   ( TextureFilteringMode fm )
{
    m_filteringMode = fm;
}

// **************************
//
void                    DefaultTexturesData::SetBorderColor     ( const glm::vec4 & bc )
{
    m_borderColor = bc;
}

} //model
} //bv
