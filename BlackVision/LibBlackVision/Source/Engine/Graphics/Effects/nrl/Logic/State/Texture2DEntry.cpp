#include "stdafx.h"

#include "Texture2DEntry.h"


namespace bv { namespace nrl {

// **************************
//
Texture2DEntry::Texture2DEntry    ()
{
}

// **************************
//
Texture2DEntry::Texture2DEntry    ( Texture2DPtr tex, const std::string & samplerName, TextureWrappingMode wrapX, TextureWrappingMode wrapY, TextureFilteringMode filteringMode )
    : m_texture( tex )
    , m_samplerEntry( samplerName, wrapX, wrapY, filteringMode )
{
}

// **************************
//
Texture2DPtr            Texture2DEntry::GetTexture     () const
{
    return m_texture;
}

// **************************
//
const std::string &     Texture2DEntry::GetSamplerName () const
{
    return m_samplerEntry.GetSamplerName();
}

// **************************
//
TextureWrappingMode     Texture2DEntry::GetWrappingModeX() const
{
    return m_samplerEntry.GetWrappingModeX();
}

// **************************
//
TextureWrappingMode     Texture2DEntry::GetWrappingModeY() const
{
    return m_samplerEntry.GetWrappingModeY();
}
    
// **************************
//
TextureFilteringMode    Texture2DEntry::GetFilteringMode() const
{
    return m_samplerEntry.GetFilteringMode();
}

// **************************
//
void                    Texture2DEntry::SetTexture     ( Texture2DPtr tex )
{
    m_texture = tex;
}

// **************************
//
void                    Texture2DEntry::SetSamplerName ( const std::string & samplerName )
{
    return m_samplerEntry.SetSamplerName( samplerName );
}

// **************************
//
void                    Texture2DEntry::SetWrappingModeX( TextureWrappingMode wrapX )
{
    return m_samplerEntry.SetWrappingModeX( wrapX );
}

// **************************
//
void                    Texture2DEntry::SetWrappingModeY( TextureWrappingMode wrapY )
{
    return m_samplerEntry.SetWrappingModeX( wrapY );
}

// **************************
//
void                    Texture2DEntry::SetFilteringMode( TextureFilteringMode filteringMode )
{
    return m_samplerEntry.SetFilteringMode( filteringMode );
}

// **************************
//
void                    Texture2DEntry::Set            ( Texture2DPtr tex, const std::string & samplerName, TextureWrappingMode wrapX, TextureWrappingMode wrapY, TextureFilteringMode filteringMode )
{
    SetTexture( tex );
    m_samplerEntry.Set( samplerName, wrapX, wrapY, filteringMode );
}

} // nrl
} // bv
