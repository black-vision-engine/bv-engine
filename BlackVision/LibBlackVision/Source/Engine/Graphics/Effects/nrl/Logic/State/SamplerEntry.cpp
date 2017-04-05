#include "stdafx.h"

#include "SamplerEntry.h"


namespace bv { namespace nrl {

// **************************
//
SamplerEntry::SamplerEntry    ()
{
}

// **************************
//
SamplerEntry::SamplerEntry    ( const std::string & samplerName, TextureWrappingMode wrapX, TextureWrappingMode wrapY, TextureFilteringMode filteringMode )
    : m_samplerName( samplerName )
    , m_wrapX( wrapX )
    , m_wrapY( wrapY )
    , m_filteringMode( filteringMode )
{
}

// **************************
//
const std::string &     SamplerEntry::GetSamplerName () const
{
    return m_samplerName;
}

// **************************
//
TextureWrappingMode     SamplerEntry::GetWrappingModeX() const
{
    return m_wrapX;
}

// **************************
//
TextureWrappingMode     SamplerEntry::GetWrappingModeY() const
{
    return m_wrapY;
}
    
// **************************
//
TextureFilteringMode    SamplerEntry::GetFilteringMode() const
{
    return m_filteringMode;
}

// **************************
//
void                    SamplerEntry::SetSamplerName ( const std::string & samplerName )
{
    m_samplerName = samplerName;
}

// **************************
//
void                    SamplerEntry::SetWrappingModeX( TextureWrappingMode wrapX )
{
    m_wrapX = wrapX;
}

// **************************
//
void                    SamplerEntry::SetWrappingModeY( TextureWrappingMode wrapY )
{
    m_wrapY = wrapY;
}

// **************************
//
void                    SamplerEntry::SetFilteringMode( TextureFilteringMode filteringMode )
{
    m_filteringMode = filteringMode;
}

// **************************
//
void                    SamplerEntry::Set            ( const std::string & samplerName, TextureWrappingMode wrapX, TextureWrappingMode wrapY, TextureFilteringMode filteringMode )
{
    SetSamplerName( samplerName );
    SetWrappingModeX( wrapX );
    SetWrappingModeY( wrapY );
    SetFilteringMode( filteringMode );
}

} // nrl
} // bv
