#include "NTexture2DEntry.h"


namespace bv { namespace nrl {

// **************************
//
NTexture2DEntry::NTexture2DEntry    ()
{
}

// **************************
//
NTexture2DEntry::NTexture2DEntry    ( Texture2DPtr tex, const std::string & samplerName, TextureWrappingMode wrapX, TextureWrappingMode wrapY, TextureFilteringMode filteringMode )
    : m_texture( tex )
    , m_samplerName( samplerName )
    , m_wrapX( wrapX )
    , m_wrapY( wrapY )
    , m_filteringMode( filteringMode )
{
}

// **************************
//
Texture2DPtr            NTexture2DEntry::GetTexture     () const
{
    return m_texture;
}

// **************************
//
const std::string &     NTexture2DEntry::GetSamplerName () const
{
    return m_samplerName;
}

// **************************
//
TextureWrappingMode     NTexture2DEntry::GetWrappingModeX() const
{
    return m_wrapX;
}

// **************************
//
TextureWrappingMode     NTexture2DEntry::GetWrappingModeY() const
{
    return m_wrapY;
}
    
// **************************
//
TextureFilteringMode    NTexture2DEntry::GetFilteringMode() const
{
    return m_filteringMode;
}

// **************************
//
void                    NTexture2DEntry::SetTexture     ( Texture2DPtr tex )
{
    m_texture = tex;
}

// **************************
//
void                    NTexture2DEntry::SetSamplerName ( const std::string & samplerName )
{
    m_samplerName = samplerName;
}

// **************************
//
void                    NTexture2DEntry::SetWrappingModeX( TextureWrappingMode wrapX )
{
    m_wrapX = wrapX;
}

// **************************
//
void                    NTexture2DEntry::SetWrappingModeY( TextureWrappingMode wrapY )
{
    m_wrapY = wrapY;
}

// **************************
//
void                    NTexture2DEntry::SetFilteringMode( TextureFilteringMode filteringMode )
{
    m_filteringMode = filteringMode;
}

// **************************
//
void                    NTexture2DEntry::Set            ( Texture2DPtr tex, const std::string & samplerName, TextureWrappingMode wrapX, TextureWrappingMode wrapY, TextureFilteringMode filteringMode )
{
    SetTexture( tex );
    SetSamplerName( samplerName );
    SetWrappingModeX( wrapX );
    SetWrappingModeY( wrapY );
    SetFilteringMode( filteringMode );
}

} // nrl
} // bv
