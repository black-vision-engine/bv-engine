#include "stdafx.h"

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
    , m_samplerEntry( samplerName, wrapX, wrapY, filteringMode )
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
    return m_samplerEntry.GetSamplerName();
}

// **************************
//
TextureWrappingMode     NTexture2DEntry::GetWrappingModeX() const
{
    return m_samplerEntry.GetWrappingModeX();
}

// **************************
//
TextureWrappingMode     NTexture2DEntry::GetWrappingModeY() const
{
    return m_samplerEntry.GetWrappingModeY();
}
    
// **************************
//
TextureFilteringMode    NTexture2DEntry::GetFilteringMode() const
{
    return m_samplerEntry.GetFilteringMode();
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
    return m_samplerEntry.SetSamplerName( samplerName );
}

// **************************
//
void                    NTexture2DEntry::SetWrappingModeX( TextureWrappingMode wrapX )
{
    return m_samplerEntry.SetWrappingModeX( wrapX );
}

// **************************
//
void                    NTexture2DEntry::SetWrappingModeY( TextureWrappingMode wrapY )
{
    return m_samplerEntry.SetWrappingModeX( wrapY );
}

// **************************
//
void                    NTexture2DEntry::SetFilteringMode( TextureFilteringMode filteringMode )
{
    return m_samplerEntry.SetFilteringMode( filteringMode );
}

// **************************
//
void                    NTexture2DEntry::Set            ( Texture2DPtr tex, const std::string & samplerName, TextureWrappingMode wrapX, TextureWrappingMode wrapY, TextureFilteringMode filteringMode )
{
    SetTexture( tex );
    m_samplerEntry.Set( samplerName, wrapX, wrapY, filteringMode );
}

} // nrl
} // bv
