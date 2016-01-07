#include "FullscreenEffectData.h"

#include <cassert>


namespace bv {

// **************************
//
FullscreenEffectData::FullscreenEffectData          ()
{
}

// **************************
//
FullscreenEffectData::~FullscreenEffectData         ()
{
}

// **************************
//
void    FullscreenEffectData::AppendInputTexture    ( Texture2DPtr tex, const std::string & samplerName )
{
    m_inputTextures.push_back( tex );
    m_samplerNames.push_back( samplerName );
}

// **************************
//
Texture2DPtr    FullscreenEffectData::GetInputTextureAt       ( unsigned int i ) const
{
    assert( i < m_inputTextures.size() );

    return m_inputTextures[ i ];
}

// **************************
//
std::string     FullscreenEffectData::GetSamplerNameAt        ( unsigned int i ) const
{
    assert( i < m_samplerNames.size() );

    return m_samplerNames[ i ];   
}

// **************************
//
unsigned int    FullscreenEffectData::GetNumTextures          () const
{
    return (unsigned int) m_inputTextures.size();
}

// **************************
//
void            FullscreenEffectData::AppendValue             ( IValuePtr val )
{
    m_values.push_back( val );
}

// **************************
//
IValuePtr       FullscreenEffectData::GetValueAt              ( unsigned int i ) const
{
    assert( i < m_values.size() );

    return m_values[ i ];
}

// **************************
//
unsigned int    FullscreenEffectData::GetNumValues            () const
{
    return (unsigned int) m_values.size();
}

// **************************
//
void            FullscreenEffectData::SetPixelShaderSource    ( const std::string & src )
{
    m_pixelShaderSource = src;
}

// **************************
//
std::string     FullscreenEffectData::GetPixelShaderSource    () const
{
    assert( m_pixelShaderSource != "" );

    return m_pixelShaderSource;
}

} //bv
