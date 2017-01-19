#include "stdafx.h"

#include "NSamplerEntry.h"


namespace bv { namespace nrl {

// **************************
//
NSamplerEntry::NSamplerEntry    ()
{
}

// **************************
//
NSamplerEntry::NSamplerEntry    ( const std::string & samplerName, TextureWrappingMode wrapX, TextureWrappingMode wrapY, TextureFilteringMode filteringMode )
    : m_samplerName( samplerName )
    , m_wrapX( wrapX )
    , m_wrapY( wrapY )
    , m_filteringMode( filteringMode )
{
}

// **************************
//
const std::string &     NSamplerEntry::GetSamplerName () const
{
    return m_samplerName;
}

// **************************
//
TextureWrappingMode     NSamplerEntry::GetWrappingModeX() const
{
    return m_wrapX;
}

// **************************
//
TextureWrappingMode     NSamplerEntry::GetWrappingModeY() const
{
    return m_wrapY;
}
    
// **************************
//
TextureFilteringMode    NSamplerEntry::GetFilteringMode() const
{
    return m_filteringMode;
}

// **************************
//
void                    NSamplerEntry::SetSamplerName ( const std::string & samplerName )
{
    m_samplerName = samplerName;
}

// **************************
//
void                    NSamplerEntry::SetWrappingModeX( TextureWrappingMode wrapX )
{
    m_wrapX = wrapX;
}

// **************************
//
void                    NSamplerEntry::SetWrappingModeY( TextureWrappingMode wrapY )
{
    m_wrapY = wrapY;
}

// **************************
//
void                    NSamplerEntry::SetFilteringMode( TextureFilteringMode filteringMode )
{
    m_filteringMode = filteringMode;
}

// **************************
//
void                    NSamplerEntry::Set            ( const std::string & samplerName, TextureWrappingMode wrapX, TextureWrappingMode wrapY, TextureFilteringMode filteringMode )
{
    SetSamplerName( samplerName );
    SetWrappingModeX( wrapX );
    SetWrappingModeY( wrapY );
    SetFilteringMode( filteringMode );
}

} // nrl
} // bv
